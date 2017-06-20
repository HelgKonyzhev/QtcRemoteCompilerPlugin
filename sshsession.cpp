#include "sshsession.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <sys/stat.h>
#include <fcntl.h>

// Good chunk size. libssh documentation recommends.
#define MAX_XFER_BUF_SIZE 16384


SSHSession::SSHSession(QObject *parent)
    : QObject(parent)
{
    m_session = NULL;
    m_channel = NULL;
    m_sftp = NULL;
    m_mode = IdleMode;
}

SSHSession::~SSHSession()
{
    closeChannel();
    disconnect();
    finish();
}

void SSHSession::start()
{
    finish();
    m_session = ssh_new();
}

void SSHSession::finish()
{
    if(m_session) {
        ssh_free(m_session);
        m_session = NULL;
    }
}

void SSHSession::setTimeout(const unsigned &timeout_usec)
{
    ssh_options_set(m_session, SSH_OPTIONS_TIMEOUT_USEC, &timeout_usec);
}

int SSHSession::connect(const QString &host)
{
    ssh_options_set(m_session, SSH_OPTIONS_HOST, host.toUtf8().data());
    int res = ssh_connect(m_session);
    emit connected(res, QString::fromLatin1(ssh_get_error(m_session)));
    return res;
}

void SSHSession::disconnect()
{
    ssh_disconnect(m_session);
}

int SSHSession::authentication(const QString &user, const QString &password)
{
    int res = ssh_userauth_password(m_session, user.toUtf8().data(), password.toUtf8().data());
    emit authenticated(res, QString::fromLatin1(ssh_get_error(m_session)));
    return res;
}

void SSHSession::createChannel()
{
    deleteChannel();
    m_channel = ssh_channel_new(m_session);
}

void SSHSession::deleteChannel()
{
    if(m_channel) {
        ssh_channel_free(m_channel);
        m_channel = NULL;
    }
}

int SSHSession::openChannel()
{
    return ssh_channel_open_session(m_channel);
}

void SSHSession::closeChannel()
{
    ssh_channel_send_eof(m_channel);
    ssh_channel_close(m_channel);
}

int SSHSession::requestExec(const QString &command)
{
    QString response;
    int res = ssh_channel_request_exec(m_channel, command.toUtf8().data());
    if(res == SSH_OK) {
        char r[1024];
        int gotBytes;
        while((gotBytes = ssh_channel_read(m_channel, r, sizeof(r), 0)) > 0)
            response.append(QString::fromUtf8(r, gotBytes));
        emit requesExecuted(response, res, QString::fromLatin1(ssh_get_error(m_session)));
    }
    return res;
}

int SSHSession::startSftp()
{
    finishSftp();
    m_sftp = sftp_new(m_session);
    int res = SSH_OK;
    if(!m_sftp)
        res = SSH_ERROR;
    emit sftpStarted(res, ssh_get_error(m_session));
    return res;
}

int SSHSession::initSftp()
{
    int res = sftp_init(m_sftp);
    emit sftpInitialized(res, sftp_get_error(m_sftp));
    return res;
}

void SSHSession::finishSftp()
{
    if(m_sftp) {
        sftp_free(m_sftp);
        m_sftp = NULL;
    }
}

int SSHSession::createDirectory(const QString &dir)
{
    int res = sftp_mkdir(m_sftp, dir.toUtf8().data(), S_IRWXU);
    if(res != SSH_OK && sftp_get_error(m_sftp) == SSH_FX_FILE_ALREADY_EXISTS)
        res = SSH_OK;
    emit directoryCreated(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
    return res;
}

static QString generateTmpFilename(const QFileInfo &fileInfo)
{
    QDateTime now = QDateTime::currentDateTime();
    QString prefix = QString::number(now.toTime_t()) + QString::number(now.time().msec()) + "_";
    return fileInfo.absoluteDir().absolutePath() + "/" + prefix + fileInfo.fileName();
}

//static QString extractFilename(const QFileInfo &tmpFileInfo)
//{
//    QStringList filenameParts = tmpFileInfo.fileName().split('_');
//    if(filenameParts.size() > 1) {
//        QString filename = filenameParts[1];
//        return tmpFileInfo.absoluteDir().absolutePath() + "/" + filename;
//    }
//    return tmpFileInfo.absoluteFilePath();
//}

int SSHSession::sendFile(const QString &source, const QString &destination)
{
    QFile f(source);
    if(!f.exists())
        throw FileNotExistException();
    if(!f.open(QIODevice::ReadOnly))
        throw FileNotOpenedException();

    QByteArray sourceData = f.readAll();
    f.close();

    QFileInfo destInfo(destination);
    int res = createDirectory(destInfo.absoluteDir().absolutePath().toUtf8().data());

    if(res != SSH_OK)
        return res;

    QString tmpDest = generateTmpFilename(destInfo);
    sftp_file destFile = sftp_open(m_sftp, tmpDest.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

    if(destFile == NULL) {
        res = SSH_ERROR;
        emit fileSent(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    int written = sftp_write(destFile, sourceData.data(), sourceData.size());

    if(written != sourceData.size()) {
        res = SSH_ERROR;
        emit fileSent(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    if((res = sftp_close(destFile)) != SSH_OK) {
        emit fileSent(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    if((res = requestExec("mv " + tmpDest + " " + destination)) != SSH_OK) {
        emit fileSent(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    emit fileSent(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
    return res;
}

int SSHSession::downloadFile(const QString &source, const QString &destination)
{
    int res;

    QString tmpDestination = generateTmpFilename(destination);
    QFile destinationFile(tmpDestination);

    if(!destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw FileNotOpenedException();

    sftp_file sourceFile = sftp_open(m_sftp, source.toUtf8().data(), O_RDONLY, 0);

    if(sourceFile == NULL) {
        res = SSH_ERROR;
        emit fileDownloaded(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    char buf[MAX_XFER_BUF_SIZE];
    int read;

    while((read = sftp_read(sourceFile, buf, sizeof(buf))) > 0) {
        int written = destinationFile.write(buf, read);
        if(written != read) {
            sftp_close(sourceFile);
            destinationFile.close();
            throw FileNotWrittenException();
        }
    }

    if((res = sftp_close(sourceFile)) != SSH_OK) {
        emit fileDownloaded(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
        return res;
    }

    if(!destinationFile.rename(tmpDestination, destination)) {
        destinationFile.close();
        throw FileNotWrittenException();
    }

    destinationFile.close();
    emit fileDownloaded(res, sftp_get_error(m_sftp), ssh_get_error(m_session));
    return res;
}

int SSHSession::startMode(Mode mode, const QString &user, const QString &host, const QString &password, const unsigned &timeout_usec)
{
    m_mode = mode;
    int res;

    start();

    if(timeout_usec > 0)
        setTimeout(timeout_usec);

    if((res = connect(host) != SSH_OK))
        return res;

    if((res = authentication(user, password) != SSH_OK))
        return res;

    if(m_mode & ChannelMode) {
        createChannel();
        if((res = openChannel()) != SSH_OK)
            return res;
    }

    if(m_mode & SFTPMode) {
        if((res = startSftp()) != SSH_OK)
            return res;

        if((res = initSftp()) != SSH_OK)
            return res;
    }

    return res;
}

void SSHSession::finishMode()
{
    if(m_mode & ChannelMode) {
        closeChannel();
        deleteChannel();
    }

    if(m_mode & SFTPMode)
        finishSftp();

    disconnect();
    finish();
}

#ifndef SSHSESSION_H
#define SSHSESSION_H

#include <QObject>
#include <libssh/libssh.h>
#include <libssh/sftp.h>


class FileNotExistException {};
class FileNotOpenedException {};
class FileNotWrittenException {};


class SSHSession : public QObject
{
    Q_OBJECT

public:
    enum Mode {
        IdleMode    = 0x0,
        ChannelMode = 0x1,
        SFTPMode    = 0x3
    };

private:
    ssh_session m_session;
    ssh_channel m_channel;
    sftp_session m_sftp;
    Mode m_mode;

public:
    SSHSession(QObject *parent = NULL);
    virtual ~SSHSession();

    void start();
    void finish();
    void setTimeout(const unsigned &timeout_usec);
    int connect(const QString &host);
    void disconnect();
    int authentication(const QString &user, const QString &password);
    void createChannel();
    void deleteChannel();
    int openChannel();
    void closeChannel();
    int requestExec(const QString &command);
    int startSftp();
    int initSftp();
    void finishSftp();
    int createDirectory(const QString &dir);
    int sendFile(const QString &source, const QString &destination);
    int downloadFile(const QString &source, const QString &destination);
    int startMode(Mode mode, const QString &user, const QString &host, const QString &password, const unsigned &timeout_usec = 0);
    void finishMode();

signals:
    void connected(int res, QString error);
    void authenticated(int res, QString error);
    void requesExecuted(QString response, int res, QString error);
    void sftpStarted(int res, QString error);
    void sftpInitialized(int res, int sftpError);
    void directoryCreated(int res, int sftpError, QString error);
    void fileSent(int res, int sftpError, QString error);
    void fileDownloaded(int res, int sftpError, QString error);
};

#endif // SSHSESSION_H

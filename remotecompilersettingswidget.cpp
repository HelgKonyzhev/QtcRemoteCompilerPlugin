#include "remotecompilersettingswidget.h"
#include "ui_remotecompilersettingswidget.h"
#include "addcompilationhostdialog.h"

#include <QHeaderView>

namespace RemoteCompiler {
namespace Internal {

void CHTModel::setHosts(const QVector<CompilationHostInfo> &hosts)
{
    beginResetModel();
    m_hosts = hosts;
    endResetModel();
}

QVariant CHTModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return m_hosts[index.row()].user;
        case 1:
            return m_hosts[index.row()].server;
        case 2:
            return QString();
        }
    } else if(role == Qt::ToolTipRole) {
        return QString::fromLatin1("Host: %1\nSysroot: %2\ng++: %3\nqmake: %4\nAbi: %5").arg(m_hosts[index.row()].user + QString::fromUtf8("@") + m_hosts[index.row()].server,
                m_hosts[index.row()].sysroot,
                m_hosts[index.row()].compiler,
                m_hosts[index.row()].qmake,
                m_hosts[index.row()].abi.toString());
    }
    return QVariant();
}

QVariant CHTModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("User");
        case 1:
            return tr("Server");
        case 2:
            return tr("Status");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int CHTModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_hosts.size();
}

int CHTModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}


RemoteCompilerSettingsWidget::RemoteCompilerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    m_config(RemoteCompilerConfigurations::currentConfig()),
    m_ui(new Ui::RemoteCompilerSettingsWidget)
{
    m_ui->setupUi(this);
    m_selectedCHIndex = -1;

    m_CHTModel.setHosts(m_config.compilationHosts());
    m_ui->createKitsCheckBox->setChecked(m_config.autoCreatingKits());

    m_ui->CHTableView->setModel(&m_CHTModel);
    m_ui->CHTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_ui->CHTableView->verticalHeader()->setVisible(false);
    m_ui->CHWarningIcon->setVisible(false);
    m_ui->CHWarningLabel->setVisible(false);
}

RemoteCompilerSettingsWidget::~RemoteCompilerSettingsWidget()
{
    delete m_ui;
}

void RemoteCompilerSettingsWidget::updateCompilationHosts(const QVector<CompilationHostInfo> &newhosts)
{
    m_config.setCompilationHosts(newhosts);
    m_CHTModel.setHosts(newhosts);
}

void RemoteCompilerSettingsWidget::enableCHControls(bool b)
{
    m_ui->removeCHButton->setEnabled(b);
    m_ui->editCHButton->setEnabled(b);
}

bool RemoteCompilerSettingsWidget::assertHostCorrect(const CompilationHostInfo &host)
{
    bool ret;
    if(!(ret = !m_config.compilationHosts().contains(host))) {
        m_ui->CHWarningLabel->setText(tr("Host ") +
                                      host.hostInfoStr() +
                                      QString::fromLatin1(" : ") + host.abi.toString() +
                                      tr(" exists"));
    }
    m_ui->CHWarningIcon->setVisible(!ret);
    m_ui->CHWarningLabel->setVisible(!ret);
    return ret;
}

void RemoteCompilerSettingsWidget::CHSelected(const QModelIndex &index)
{
    m_selectedCHIndex = index.row();
    enableCHControls(true);
}

void RemoteCompilerSettingsWidget::editCH()
{
    QVector<CompilationHostInfo> hosts = m_config.compilationHosts();
    CompilationHostInfo editedHost = hosts[m_selectedCHIndex];
    const CompilationHostInfo &notEditedHost = hosts[m_selectedCHIndex];
    int res = AddCompilationHostDialog::getHostInfo(editedHost);
    if(res == QDialog::Accepted) {
        if(editedHost != notEditedHost && !assertHostCorrect(editedHost))
            return;
        hosts[m_selectedCHIndex] = editedHost;
        updateCompilationHosts(hosts);
        m_ui->CHTableView->selectRow(m_selectedCHIndex);
    }
}

void RemoteCompilerSettingsWidget::saveSettings()
{
    RemoteCompilerConfigurations::setConfig(m_config);
}

void RemoteCompilerSettingsWidget::on_addCHButton_clicked()
{
    CompilationHostInfo host;
    int res = AddCompilationHostDialog::getHostInfo(host);
    if(res == QDialog::Accepted) {
        QVector<CompilationHostInfo> hosts = m_config.compilationHosts();
        if(!assertHostCorrect(host))
            return;
        hosts.append(host);
        updateCompilationHosts(hosts);
        if(m_selectedCHIndex != -1) {
            m_selectedCHIndex = hosts.size() - 1;
            m_ui->CHTableView->selectRow(m_selectedCHIndex);
        }
    }
}

void RemoteCompilerSettingsWidget::on_removeCHButton_clicked()
{
    QVector<CompilationHostInfo> hosts = m_config.compilationHosts();
    hosts.remove(m_selectedCHIndex);
    updateCompilationHosts(hosts);
    if(m_selectedCHIndex != -1) {
        if(hosts.size() > 0) {
            m_ui->CHTableView->selectRow(m_selectedCHIndex == 0 ? 0 : --m_selectedCHIndex);
        } else {
            m_selectedCHIndex = -1;
            enableCHControls(false);
        }
    }
}

void RemoteCompilerSettingsWidget::on_createKitsCheckBox_stateChanged(int arg1)
{
    m_config.setAutoCreatingKits(arg1 == Qt::Checked);
}

void RemoteCompilerSettingsWidget::on_CHTableView_clicked(const QModelIndex &index)
{
    CHSelected(index);
}

void RemoteCompilerSettingsWidget::on_editCHButton_clicked()
{
    editCH();
}

void RemoteCompilerSettingsWidget::on_CHTableView_doubleClicked(const QModelIndex &index)
{
    CHSelected(index);
    editCH();
}

} //Internal
} //RemoteCompiler

#include "addcompilationhostdialog.h"
#include "ui_addcompilationhostdialog.h"

#include <QPushButton>
#include <QDebug>

namespace RemoteCompiler {
namespace Internal {

using namespace ProjectExplorer;

AddCompilationHostDialog::AddCompilationHostDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AddCompilationHostDialog)
{
    m_ui->setupUi(this);
    m_ui->addCNButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    m_ui->inputServerLineEdit->setInputMask(QString::fromUtf8("000.000.000.000"));
    m_ui->inputPasswordLineEdit->setEchoMode(QLineEdit::Password);

    m_abiWidget = new AbiWidget(this);
    m_ui->settingsLayout->addRow(new QLabel(tr("Abi:"), this), m_abiWidget);

    m_addingState = NothigSet;
}

AddCompilationHostDialog::~AddCompilationHostDialog()
{
    delete m_ui;
}

void AddCompilationHostDialog::on_inputUserLineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
        m_addingState |= UserSet;
    else
        m_addingState &= ~UserSet;
    CNDataChanged();
}

void AddCompilationHostDialog::on_inputServerLineEdit_textChanged(const QString &arg1)
{
    QRegExp srvRegEx(QString::fromUtf8("^[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}$"));
    if(srvRegEx.exactMatch(arg1))
        m_addingState |= ServerSet;
    else
        m_addingState &= ~ServerSet;
    CNDataChanged();
}

void AddCompilationHostDialog::on_inputPasswordLineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
        m_addingState |= PasswordSet;
    else
        m_addingState &= ~PasswordSet;
    CNDataChanged();
}

void AddCompilationHostDialog::CNDataChanged()
{
    m_ui->addCNButtonBox->button(QDialogButtonBox::Ok)->setEnabled((m_addingState & UserSet) &&
                                                                   (m_addingState & ServerSet) &&
                                                                   (m_addingState & PasswordSet));
}

int AddCompilationHostDialog::exec(CompilationHostInfo &info)
{
    m_ui->inputUserLineEdit->setText(info.user);
    m_ui->inputServerLineEdit->setText(info.server);
    m_ui->inputPasswordLineEdit->setText(info.password);
    m_ui->inputSysrootLineEdit->setText(info.sysroot);
    m_ui->inputCompilerLineEdit->setText(info.compiler);
    m_ui->inputQmakeLineEdit->setText(info.qmake);
    m_abiWidget->setAbis(m_abiWidget->supportedAbis(), info.abi);

    int ret = QDialog::exec();

    info.user = m_ui->inputUserLineEdit->text();
    info.server = m_ui->inputServerLineEdit->text();
    info.password = m_ui->inputPasswordLineEdit->text();
    info.sysroot = m_ui->inputSysrootLineEdit->text();
    info.compiler = m_ui->inputCompilerLineEdit->text();
    info.qmake = m_ui->inputQmakeLineEdit->text();
    info.abi = m_abiWidget->currentAbi();

    return ret;
}

int AddCompilationHostDialog::getHostInfo(CompilationHostInfo &info, bool enabled)
{
    AddCompilationHostDialog *d = new AddCompilationHostDialog;
    d->setEnabled(enabled);
    int ret = d->exec(info);
    delete d;
    return ret;
}


} //Internal
} //RemoteCompiler

#include "addcompilationnodedialog.h"
#include "ui_addcompilationnodedialog.h"

#include <QPushButton>
#include <QDebug>

namespace RemoteCompiler {
namespace Internal {

AddCompilationNodeDialog::AddCompilationNodeDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AddCompilationNodeDialog)
{
    m_ui->setupUi(this);
    m_ui->addCNButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    m_ui->inputServerLineEdit->setInputMask(QString::fromUtf8("000.000.000.000"));
    m_ui->inputPasswordLineEdit->setEchoMode(QLineEdit::Password);
    m_addingState = NothigSet;
}

AddCompilationNodeDialog::~AddCompilationNodeDialog()
{
    delete m_ui;
}

void AddCompilationNodeDialog::on_inputUserLineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
        m_addingState |= UserSet;
    else
        m_addingState &= ~UserSet;
    CNDataChanged();
}

void AddCompilationNodeDialog::on_inputServerLineEdit_textChanged(const QString &arg1)
{
    QRegExp srvRegEx(QString::fromUtf8("^[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}.[0-9]{1,3}[ ]{0,2}$"));
    if(srvRegEx.exactMatch(arg1))
        m_addingState |= ServerSet;
    else
        m_addingState &= ~ServerSet;
    CNDataChanged();
}

void AddCompilationNodeDialog::on_inputPasswordLineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
        m_addingState |= PasswordSet;
    else
        m_addingState &= ~PasswordSet;
    CNDataChanged();
}

void AddCompilationNodeDialog::CNDataChanged()
{
    m_ui->addCNButtonBox->button(QDialogButtonBox::Ok)->setEnabled((m_addingState & UserSet) &&
                                                                   (m_addingState & ServerSet) &&
                                                                   (m_addingState & PasswordSet));
}

int AddCompilationNodeDialog::exec(CompilationNodeInfo &info)
{
    m_ui->inputUserLineEdit->setText(info.user);
    m_ui->inputServerLineEdit->setText(info.server);
    m_ui->inputPasswordLineEdit->setText(info.password);

    int ret = QDialog::exec();

    info.user = m_ui->inputUserLineEdit->text();
    info.server = m_ui->inputServerLineEdit->text();
    info.password = m_ui->inputPasswordLineEdit->text();

    return ret;
}

int AddCompilationNodeDialog::getNodeInfo(CompilationNodeInfo &info)
{
    AddCompilationNodeDialog *d = new AddCompilationNodeDialog;
    int ret = d->exec(info);
    delete d;
    return ret;
}

} //Internal
} //RemoteCompiler


#ifndef ADDCOMPILATIONHOSTDIALOG_H
#define ADDCOMPILATIONHOSTDIALOG_H

#include "remotecompilerconfigurations.h"

#include <QDialog>
#include <projectexplorer/abiwidget.h>

namespace Ui {
class AddCompilationHostDialog;
}

namespace RemoteCompiler {
namespace Internal {

enum AddingState : unsigned char {
    NothigSet   = 0x0,
    UserSet     = 0x1,
    ServerSet   = 0x2,
    PasswordSet = 0x4,
};

class AddCompilationHostDialog : public QDialog
{
    Q_OBJECT

    unsigned char m_addingState;
    ProjectExplorer::AbiWidget *m_abiWidget;

    void CNDataChanged();

public:
    explicit AddCompilationHostDialog(QWidget *parent = 0);
    ~AddCompilationHostDialog();

    int exec(CompilationHostInfo &info);
    static int getHostInfo(CompilationHostInfo &info, bool enabled = true);

private slots:
    void on_inputUserLineEdit_textChanged(const QString &arg1);
    void on_inputServerLineEdit_textChanged(const QString &arg1);
    void on_inputPasswordLineEdit_textChanged(const QString &arg1);

private:
    Ui::AddCompilationHostDialog *m_ui;
};

} //Internal
} //RemoteCompiler

#endif // ADDCOMPILATIONHOSTDIALOG_H

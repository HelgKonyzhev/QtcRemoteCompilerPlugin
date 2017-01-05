#ifndef ADDCOMPILATIONNODEDIALOG_H
#define ADDCOMPILATIONNODEDIALOG_H

#include "remotecompilerconfigurations.h"

#include <QDialog>

namespace Ui {
class AddCompilationNodeDialog;
}

namespace RemoteCompiler {
namespace Internal {

enum AddingState : unsigned char {
    NothigSet   = 0x0,
    UserSet     = 0x1,
    ServerSet   = 0x2,
    PasswordSet = 0x4,
};

class AddCompilationNodeDialog : public QDialog
{
    Q_OBJECT

    unsigned char m_addingState;

    void CNDataChanged();

public:
    explicit AddCompilationNodeDialog(QWidget *parent = 0);
    ~AddCompilationNodeDialog();

    int exec(CompilationNodeInfo &info);
    static int getNodeInfo(CompilationNodeInfo &info);

private slots:
    void on_inputUserLineEdit_textChanged(const QString &arg1);
    void on_inputServerLineEdit_textChanged(const QString &arg1);
    void on_inputPasswordLineEdit_textChanged(const QString &arg1);

private:
    Ui::AddCompilationNodeDialog *m_ui;
};

} //Internal
} //RemoteCompiler

#endif // ADDCOMPILATIONNODEDIALOG_H

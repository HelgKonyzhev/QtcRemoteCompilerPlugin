#ifndef REMOTECOMPILERTOOLCHAIN_H
#define REMOTECOMPILERTOOLCHAIN_H

#include "remotecompilerconfigurations.h"

#include <projectexplorer/gcctoolchain.h>
#include <projectexplorer/toolchainconfigwidget.h>
#include <projectexplorer/abiwidget.h>
#include <QLineEdit>


namespace RemoteCompiler {
namespace Internal {


class RemoteCompilerToolchain : public ProjectExplorer::GccToolChain
{
    friend class RemoteCompilerToolchainFactory;

    QString m_hostInfoStr;

    explicit RemoteCompilerToolchain(const ProjectExplorer::Abi &abi, const QString &hostInfoStr, Detection d);
    RemoteCompilerToolchain();
    RemoteCompilerToolchain(const RemoteCompilerToolchain &);

public:
    ~RemoteCompilerToolchain();

    QString type() const;
    QString typeDisplayName() const;
    bool isValid() const;
    void addToEnvironment(Utils::Environment &env) const;
    bool operator ==(const ProjectExplorer::ToolChain &tc) const;
    ProjectExplorer::ToolChainConfigWidget *configurationWidget();
    virtual Utils::FileName suggestedDebugger() const;
    QVariantMap toMap() const;
    bool fromMap(const QVariantMap &data);
    QList<Utils::FileName> suggestedMkspecList() const;
    QString makeCommand(const Utils::Environment &environment) const;

protected:
    QList<ProjectExplorer::Abi> detectSupportedAbis() const;
};


class RemoteCompilerToolChainConfigWidget : public ProjectExplorer::ToolChainConfigWidget
{
    Q_OBJECT

    QLineEdit *m_compiler;
    ProjectExplorer::AbiWidget *m_abi;

public:
    RemoteCompilerToolChainConfigWidget(RemoteCompilerToolchain *);

private:
    void applyImpl() {}
    void discardImpl() {}
    bool isDirtyImpl() const;
    void makeReadOnlyImpl();
};


class RemoteCompilerToolchainFactory : public ProjectExplorer::ToolChainFactory
{
    Q_OBJECT

public:
    RemoteCompilerToolchainFactory();

    QList<ProjectExplorer::ToolChain *> autoDetect();
    bool canRestore(const QVariantMap &data);
    ProjectExplorer::ToolChain *restore(const QVariantMap &data);

    static QList<ProjectExplorer::ToolChain *> createToolChains();
};


} //RemoteCompiler
} //Internal

#endif // REMOTECOMPILERTOOLCHAIN_H

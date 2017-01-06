#include "remotecompilertoolchain.h"
#include "remotecompilerconstants.h"

#include <utils/environment.h>
#include <utils/hostosinfo.h>
#include <QFormLayout>


namespace RemoteCompiler {
namespace Internal {

using namespace ProjectExplorer;
using namespace Utils;

static const char REMOTE_COMPILER_HOST_INFO[] = "Qt4ProjectManager.RemoteCompiler.TC_VERION";

RemoteCompilerToolchain::RemoteCompilerToolchain(const Abi &abi, const QString &hostInfoStr, Detection d)
    : GccToolChain(QLatin1String(Constants::REMOTE_COMPILER_TOOLCHAIN_ID), d),
      m_hostInfoStr(hostInfoStr)
{
    setTargetAbi(abi);
    setDisplayName(hostInfoStr);
}

// for fromMap
RemoteCompilerToolchain::RemoteCompilerToolchain()
    : GccToolChain(QLatin1String(Constants::REMOTE_COMPILER_TOOLCHAIN_ID), ToolChain::ManualDetection)
{
}

RemoteCompilerToolchain::RemoteCompilerToolchain(const RemoteCompilerToolchain &tc) :
    GccToolChain(tc)
{
}

RemoteCompilerToolchain::~RemoteCompilerToolchain()
{
}

QString RemoteCompilerToolchain::type() const
{
    return QLatin1String(Constants::REMOTE_COMPILER_TOOLCHAIN_TYPE);
}

QString RemoteCompilerToolchain::typeDisplayName() const
{
    return RemoteCompilerToolchainFactory::tr("Remote compiler");
}

bool RemoteCompilerToolchain::isValid() const
{
    return !compilerCommand().isNull();
}

void RemoteCompilerToolchain::addToEnvironment(Utils::Environment &) const
{
}

bool RemoteCompilerToolchain::operator ==(const ProjectExplorer::ToolChain &tc) const
{
    if (!GccToolChain::operator ==(tc))
        return false;

    return m_hostInfoStr == static_cast<const RemoteCompilerToolchain &>(tc).m_hostInfoStr;
}

ProjectExplorer::ToolChainConfigWidget *RemoteCompilerToolchain::configurationWidget()
{
    return new RemoteCompilerToolChainConfigWidget(this);
}

Utils::FileName RemoteCompilerToolchain::suggestedDebugger() const
{
    return FileName::fromLatin1("");
}

QVariantMap RemoteCompilerToolchain::toMap() const
{
    QVariantMap result = GccToolChain::toMap();
    result.insert(QLatin1String(REMOTE_COMPILER_HOST_INFO), m_hostInfoStr);
    return result;
}

bool RemoteCompilerToolchain::fromMap(const QVariantMap &data)
{
    if (!GccToolChain::fromMap(data))
        return false;
    m_hostInfoStr = data.value(QLatin1String(REMOTE_COMPILER_HOST_INFO)).toString();
    return isValid();
}

QList<Utils::FileName> RemoteCompilerToolchain::suggestedMkspecList() const
{
    return QList<FileName>();
}

QString RemoteCompilerToolchain::makeCommand(const Utils::Environment &environment) const
{
    QString make = QLatin1String("make");
    FileName tmp = environment.searchInPath(make);
    return tmp.isEmpty() ? make : tmp.toString();
}

QList<Abi> RemoteCompilerToolchain::detectSupportedAbis() const
{
    return QList<Abi>() << targetAbi();
}


// --------------------------------------------------------------------------
// ToolChainConfigWidget
// --------------------------------------------------------------------------

RemoteCompilerToolChainConfigWidget::RemoteCompilerToolChainConfigWidget(RemoteCompilerToolchain *tc) :
   ToolChainConfigWidget(tc)
{
    m_compiler = new QLineEdit(tc->compilerCommand().toString());
    m_mainLayout->addRow(tr("Compiler:"), m_compiler);

    m_abi = new AbiWidget;
    m_abi->setAbis(tc->supportedAbis(), tc->targetAbi());
    m_mainLayout->addRow(tr("Abi:"), m_abi);
}

bool RemoteCompilerToolChainConfigWidget::isDirtyImpl() const
{
    const QVector<CompilationHostInfo> existingHosts = RemoteCompilerConfigurations::currentConfig().compilationHosts();
    for(int i = 0; i < existingHosts.size(); i++)
        if(existingHosts[i].hostInfoStr() == m_nameLineEdit->text() &&
                existingHosts[i].abi == m_abi->currentAbi())
        {
            return false;
        }
    return true;
}

void RemoteCompilerToolChainConfigWidget::makeReadOnlyImpl()
{
    m_compiler->setReadOnly(true);
    m_abi->setEnabled(false);
}

// --------------------------------------------------------------------------
// ToolChainFactory
// --------------------------------------------------------------------------


RemoteCompilerToolchainFactory::RemoteCompilerToolchainFactory()
{
    setId(Constants::REMOTE_COMPILER_TOOLCHAIN_ID);
    setDisplayName(tr("Remote compiler"));
}

QList<ProjectExplorer::ToolChain *> RemoteCompilerToolchainFactory::autoDetect()
{
    return createToolChains();
}

bool RemoteCompilerToolchainFactory::canRestore(const QVariantMap &data)
{
    return idFromMap(data).startsWith(QLatin1String(Constants::REMOTE_COMPILER_TOOLCHAIN_ID) + QLatin1Char(':'));
}

ProjectExplorer::ToolChain *RemoteCompilerToolchainFactory::restore(const QVariantMap &data)
{
    RemoteCompilerToolchain *tc = new RemoteCompilerToolchain();
    if (tc->fromMap(data))
        return tc;

    delete tc;
    return 0;
}

QList<ToolChain *> RemoteCompilerToolchainFactory::createToolChains()
{
    QList<ToolChain *> result;
    foreach (const CompilationHostInfo &host, RemoteCompilerConfigurations::currentConfig().compilationHosts()) {
        RemoteCompilerToolchain *tc = new RemoteCompilerToolchain(host.abi,
                                                                  host.hostInfoStr(),
                                                                  ToolChain::AutoDetection);
        tc->resetToolChain(FileName::fromString(host.compiler));
        result.append(tc);
    }
    return result;
}

}
}

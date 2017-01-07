#include "remotecompilerconfigurations.h"
#include "remotecompilertoolchain.h"
#include "remotecompilerconstants.h"
#include "remotecompilerqtversion.h"

#include <coreplugin/icore.h>
#include <projectexplorer/gcctoolchain.h>
#include <projectexplorer/toolchainmanager.h>
#include <qtsupport/qtversionfactory.h>
#include <qtsupport/qtversionmanager.h>


namespace RemoteCompiler {
namespace Internal {

using namespace ProjectExplorer;
using namespace QtSupport;
using namespace Utils;

const QLatin1String SettingsGroupKey("RemoteCompilerConfigurations");
const QLatin1String CompilationHostsArrayKey("CompilationHostsArray");
const QLatin1String AutoCreatingKitsKey("AutoCreatingKits");
const QLatin1String UserKey("User");
const QLatin1String ServerKey("Server");
const QLatin1String PasswordKey("Password");
const QLatin1String SysrootKey("Sysroot");
const QLatin1String CompilerKey("Compiler");
const QLatin1String QMakeKey("QMake");
const QLatin1String AbiKey("Abi");


bool CompilationHostInfo::operator ==(const CompilationHostInfo &n) const
{
    return user == n.user && server == n.server && abi == n.abi;
}

bool CompilationHostInfo::operator !=(const CompilationHostInfo &n) const
{
    return !((*this) == n);
}

QString CompilationHostInfo::hostInfoStr() const
{
    return user + QString::fromLatin1("@") + server;
}

QString CompilationHostInfo::qtVersionData() const
{
    return QLatin1String(Constants::REMOTE_COMPILER_QT_VERSION_DATA_HEADER) + QLatin1Char(':') +
            hostInfoStr() + QLatin1Char(':') +
            abi.toString();
}

CompilationHostInfo::CompilationHostInfo()
{
    sysroot = QString::fromLatin1("/");
    compiler = QString::fromLatin1("/usr/bin/g++");
    qmake = QString::fromLatin1("/usr/bin/qmake");
}


RemoteCompilerConfig::RemoteCompilerConfig()
{

}

void RemoteCompilerConfig::load(QSettings &settings)
{
    m_autoCreatingKits = settings.value(AutoCreatingKitsKey, true).toBool();

    int nodesCount = settings.beginReadArray(CompilationHostsArrayKey);
    m_compilationHosts.resize(nodesCount);
    for(int i = 0; i < nodesCount; i++) {
        settings.setArrayIndex(i);
        m_compilationHosts[i].user = settings.value(UserKey).toString();
        m_compilationHosts[i].server = settings.value(ServerKey).toString();
        m_compilationHosts[i].password = settings.value(PasswordKey).toString();
        m_compilationHosts[i].sysroot = settings.value(SysrootKey, QString::fromLatin1("/")).toString();
        m_compilationHosts[i].compiler = settings.value(CompilerKey, QString::fromLatin1("/usr/bin/g++")).toString();
        m_compilationHosts[i].qmake = settings.value(QMakeKey, QString::fromLatin1("/usr/bin/qmake")).toString();
        m_compilationHosts[i].abi = ProjectExplorer::Abi(settings.value(AbiKey).toString());
    }
    settings.endArray();
}

void RemoteCompilerConfig::save(QSettings &settings) const
{
    settings.setValue(AutoCreatingKitsKey, m_autoCreatingKits);

    settings.beginWriteArray(CompilationHostsArrayKey, m_compilationHosts.size());
    for(int i = 0; i < m_compilationHosts.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(UserKey, m_compilationHosts[i].user);
        settings.setValue(ServerKey, m_compilationHosts[i].server);
        settings.setValue(PasswordKey, m_compilationHosts[i].password);
        settings.setValue(SysrootKey, m_compilationHosts[i].sysroot);
        settings.setValue(CompilerKey, m_compilationHosts[i].compiler);
        settings.setValue(QMakeKey, m_compilationHosts[i].qmake);
        settings.setValue(AbiKey, m_compilationHosts[i].abi.toString());
    }
    settings.endArray();
}

const bool &RemoteCompilerConfig::autoCreatingKits() const
{
    return m_autoCreatingKits;
}

void RemoteCompilerConfig::setAutoCreatingKits(bool b)
{
    m_autoCreatingKits = b;
}

const QVector<CompilationHostInfo> &RemoteCompilerConfig::compilationHosts() const
{
    return m_compilationHosts;
}

void RemoteCompilerConfig::setCompilationHosts(const QVector<CompilationHostInfo> &hosts)
{
    m_compilationHosts = hosts;
}


RemoteCompilerConfigurations::RemoteCompilerConfigurations(QObject *parent)
    : QObject(parent)
{
    load();

    m_instance = this;
}

void RemoteCompilerConfigurations::load()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroupKey);
    m_config.load(*settings);
    settings->endGroup();
}

void RemoteCompilerConfigurations::save()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroupKey);
    m_config.save(*settings);
    settings->endGroup();
}

RemoteCompilerConfigurations *RemoteCompilerConfigurations::instance()
{
    return m_instance;
}

const RemoteCompilerConfig &RemoteCompilerConfigurations::currentConfig()
{
    return m_instance->m_config;
}

void RemoteCompilerConfigurations::setConfig(const RemoteCompilerConfig &config)
{
    m_instance->m_config = config;
    m_instance->save();
    m_instance->updateToolChainList();
    m_instance->updateQtVersionList();
}

void RemoteCompilerConfigurations::updateToolChainList()
{
    QList<ToolChain *> existingToolChains = ToolChainManager::toolChains();
    QList<ToolChain *> toolchains = RemoteCompilerToolchainFactory::createToolChains();
    foreach (ToolChain *tc, toolchains) {
        bool found = false;
        for (int i = 0; i < existingToolChains.count(); ++i) {
            if (*(existingToolChains.at(i)) == *tc) {
                found = true;
                break;
            }
        }
        if (found)
            delete tc;
        else
            ToolChainManager::registerToolChain(tc);
    }

    foreach (ToolChain *tc, existingToolChains) {
        if (tc->type() == QLatin1String(Constants::REMOTE_COMPILER_TOOLCHAIN_TYPE)) {
            if (!tc->isValid())
                ToolChainManager::deregisterToolChain(tc);
        }
    }
}

void RemoteCompilerConfigurations::updateQtVersionList()
{
    foreach (const CompilationHostInfo &h, m_instance->currentConfig().compilationHosts()) {
        bool isNew = true;
        foreach (BaseQtVersion *v, QtVersionManager::versions()) {
            if(v->type() != QLatin1String(Constants::REMOTE_COMPILER_QT) || dynamic_cast<RemoteCompilerQtVersion*>(v) == 0)
                continue;

            RemoteCompilerQtVersion *rcVer = dynamic_cast<RemoteCompilerQtVersion*>(v);
            if(rcVer->isMyHost(h)) {
                if(rcVer->isOutdated(h))
                    QtVersionManager::removeVersion(rcVer);
                else
                    isNew = false;
                break;
            }
        }
        if(isNew) {
//            BaseQtVersion *v = QtVersionFactory::createQtVersionFromQMakePath(FileName::fromString(h.qmake), true, h.qtVersionData());
            BaseQtVersion *v = new RemoteCompilerQtVersion(h.hostInfoStr(), h.abi, FileName::fromString(h.qmake), true, h.qtVersionData());
            QtVersionManager::addVersion(v);
        }
    }
}


RemoteCompilerConfigurations *RemoteCompilerConfigurations::m_instance = 0;


} //Internal
} //RemoteCompiler

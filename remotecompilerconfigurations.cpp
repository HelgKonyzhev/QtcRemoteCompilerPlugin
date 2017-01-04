#include "remotecompilerconfigurations.h"

#include <coreplugin/icore.h>


namespace RemoteCompiler {
namespace Internal {

const QLatin1String SettingsGroup("RemoteCompilerConfigurations");
const QLatin1String CompilationNodesArray("CompilationNodesArray");
const QLatin1String AutoCreatingKits("AutoCreatingKits");
const QLatin1String User("User");
const QLatin1String Server("Server");
const QLatin1String Password("Password");


bool CompilationNodeInfo::operator ==(const CompilationNodeInfo &n) const
{
    return user == n.user && server == n.server;
}


RemoteCompilerConfig::RemoteCompilerConfig()
{

}

void RemoteCompilerConfig::load(QSettings &settings)
{
    m_autoCreatingKits = settings.value(AutoCreatingKits, true).toBool();

    int nodesCount = settings.beginReadArray(CompilationNodesArray);
    m_compilationNodes.resize(nodesCount);
    for(int i = 0; i < nodesCount; i++) {
        settings.setArrayIndex(i);
        m_compilationNodes[i].user = settings.value(User).toString();
        m_compilationNodes[i].server = settings.value(Server).toString();
        m_compilationNodes[i].password = settings.value(Password).toString();
    }
    settings.endArray();
}

void RemoteCompilerConfig::save(QSettings &settings) const
{
    settings.setValue(AutoCreatingKits, m_autoCreatingKits);

    settings.beginWriteArray(CompilationNodesArray, m_compilationNodes.size());
    for(int i = 0; i < m_compilationNodes.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(User, m_compilationNodes[i].user);
        settings.setValue(Server, m_compilationNodes[i].server);
        settings.setValue(Password, m_compilationNodes[i].password);
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

const QVector<CompilationNodeInfo> &RemoteCompilerConfig::compilationNodes() const
{
    return m_compilationNodes;
}

void RemoteCompilerConfig::setCompilationNodes(const QVector<CompilationNodeInfo> &nodes)
{
    m_compilationNodes = nodes;
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
    settings->beginGroup(SettingsGroup);
    m_config.load(*settings);
    settings->endGroup();
}

void RemoteCompilerConfigurations::save()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(SettingsGroup);
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
}


RemoteCompilerConfigurations *RemoteCompilerConfigurations::m_instance = 0;


} //Internal
} //RemoteCompiler

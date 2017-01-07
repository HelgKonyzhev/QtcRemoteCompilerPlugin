#ifndef REMOTECOMPILERCONFIGURATIONS_H
#define REMOTECOMPILERCONFIGURATIONS_H

#include <QString>
#include <QObject>
#include <QSettings>
#include <QVector>
#include <projectexplorer/abi.h>

namespace RemoteCompiler {
namespace Internal {

struct CompilationHostInfo
{
    QString user;
    QString server;
    QString password;
    QString sysroot;
    QString compiler;
    QString qmake;
    ProjectExplorer::Abi abi;

    bool operator ==(const CompilationHostInfo &n) const;
    bool operator !=(const CompilationHostInfo &n) const;
    QString hostInfoStr() const;
    QString qtVersionData() const;

    CompilationHostInfo();
};


class RemoteCompilerConfig
{
    QVector<CompilationHostInfo> m_compilationHosts;
    bool m_autoCreatingKits;

public:
    RemoteCompilerConfig();

    void load(QSettings &settings);
    void save(QSettings &settings) const;

    const bool &autoCreatingKits() const;
    void setAutoCreatingKits(bool b);

    const QVector<CompilationHostInfo> &compilationHosts() const;
    void setCompilationHosts(const QVector<CompilationHostInfo> &hosts);
};


class RemoteCompilerConfigurations : public QObject
{
    friend class RemoteCompilerPlugin;
    Q_OBJECT

    static RemoteCompilerConfigurations *m_instance;
    RemoteCompilerConfig m_config;

    RemoteCompilerConfigurations(QObject *parent);

    void load();
    void save();

public:
    static RemoteCompilerConfigurations *instance();
    static const RemoteCompilerConfig &currentConfig();
    static void setConfig(const RemoteCompilerConfig &config);
    static void updateToolChainList();
    static void updateQtVersionList();
};

} //Internal
} //RemoteCompiler

#endif // REMOTECOMPILERCONFIGURATIONS_H

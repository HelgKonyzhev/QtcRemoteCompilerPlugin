#ifndef REMOTECOMPILERCONFIGURATIONS_H
#define REMOTECOMPILERCONFIGURATIONS_H

#include <QString>
#include <QObject>
#include <QSettings>
#include <QVector>

namespace RemoteCompiler {
namespace Internal {

struct CompilationNodeInfo
{
    QString user;
    QString server;
    QString password;

    bool operator ==(const CompilationNodeInfo &n) const;
};


class RemoteCompilerConfig
{
    QVector<CompilationNodeInfo> m_compilationNodes;
    bool m_autoCreatingKits;

public:
    RemoteCompilerConfig();

    void load(QSettings &settings);
    void save(QSettings &settings) const;

    const bool &autoCreatingKits() const;
    void setAutoCreatingKits(bool b);

    const QVector<CompilationNodeInfo> &compilationNodes() const;
    void setCompilationNodes(const QVector<CompilationNodeInfo> &nodes);
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
};

} //Internal
} //RemoteCompiler

#endif // REMOTECOMPILERCONFIGURATIONS_H

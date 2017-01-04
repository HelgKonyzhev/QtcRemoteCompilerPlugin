#ifndef REMOTECOMPILER_H
#define REMOTECOMPILER_H

#include "remotecompiler_global.h"

#include <extensionsystem/iplugin.h>

namespace RemoteCompiler {
namespace Internal {

class RemoteCompilerPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "RemoteCompiler.json")

public:
    RemoteCompilerPlugin();
    ~RemoteCompilerPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void triggerAction();
};

} // namespace Internal
} // namespace RemoteCompiler

#endif // REMOTECOMPILER_H


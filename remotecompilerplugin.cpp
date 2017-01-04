#include "remotecompilerplugin.h"
#include "remotecompilerconstants.h"
#include "remotecompilersettingspage.h"
#include "remotecompilerconfigurations.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace RemoteCompiler::Internal;

RemoteCompilerPlugin::RemoteCompilerPlugin()
{
    // Create your members
}

RemoteCompilerPlugin::~RemoteCompilerPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool RemoteCompilerPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    new RemoteCompilerConfigurations(this);

    addAutoReleasedObject(new Internal::RemoteCompilerSettingsPage);

    return true;
}

void RemoteCompilerPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag RemoteCompilerPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void RemoteCompilerPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action triggered"),
                             tr("This is an action from RemoteCompiler."));
}


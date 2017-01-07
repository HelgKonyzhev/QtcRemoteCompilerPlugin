#ifndef REMOTECOMPILERCONSTANTS_H
#define REMOTECOMPILERCONSTANTS_H

namespace RemoteCompiler {
namespace Constants {

const char ACTION_ID[] = "RemoteCompiler.Action";
const char MENU_ID[] = "RemoteCompiler.Menu";

const char REMOTE_COMPILER_SETTINGS_ID[] = "ZZ.RemoteCompiler.Configurations";
const char REMOTE_COMPILER_SETTINGS_CATEGORY[] = "XA.RemoteCompiler.";
const char REMOTE_COMPILER_SETTINGS_TR_CATEGORY[] = QT_TRANSLATE_NOOP("Remote compiler", "Remote compiler");
const char REMOTE_COMPILER_SETTINGS_CATEGORY_ICON[] = ":/remotecompiler/images/qtc_remote_compiler.png";

const char REMOTE_COMPILER_TOOLCHAIN_TYPE[] = "remote";
const char REMOTE_COMPILER_TOOLCHAIN_ID[] = "Qt4ProjectManager.ToolChain.RemoteCompiler";
const char REMOTE_COMPILER_QT[] = "Qt4ProjectManager.QtVersion.RemoteCompiler";
const char REMOTE_COMPILER_QT_VERSION_DATA_HEADER[] = "RemoteQtVersion";

} // namespace RemoteCompiler
} // namespace Constants

#endif // REMOTECOMPILERCONSTANTS_H


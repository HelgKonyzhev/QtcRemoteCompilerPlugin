DEFINES += REMOTECOMPILER_LIBRARY

# RemoteCompiler files

SOURCES += remotecompilerplugin.cpp \
    remotecompilersettingspage.cpp \
    remotecompilersettingswidget.cpp \
    remotecompilerconfigurations.cpp \
    addcompilationhostdialog.cpp \
    remotecompilertoolchain.cpp \
    remotecompilerqtversionfactory.cpp \
    remotecompilerqtversion.cpp

HEADERS += remotecompilerplugin.h \
        remotecompiler_global.h \
        remotecompilerconstants.h \
    remotecompilersettingspage.h \
    remotecompilersettingswidget.h \
    remotecompilerconfigurations.h \
    addcompilationhostdialog.h \
    remotecompilertoolchain.h \
    remotecompilerqtversionfactory.h \
    remotecompilerqtversion.h

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/home/edelweiss/tmp/qt-creator-opensource-src-3.4.1

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/home/edelweiss/tmp/qt-creator-opensource-src-3.4.1/build

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = RemoteCompiler
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    projectexplorer \
    qtsupport

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

RESOURCES += \
    remotecpmpiler.qrc

FORMS += \
    remotecompilersettingswidget.ui \
    addcompilationhostdialog.ui


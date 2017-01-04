#include "remotecompilersettingspage.h"
#include "remotecompilerconstants.h"
#include "remotecompilersettingswidget.h"

#include <QCoreApplication>

namespace RemoteCompiler {
namespace Internal {

RemoteCompilerSettingsPage::RemoteCompilerSettingsPage(QObject *parent)
    : Core::IOptionsPage(parent)
{
    setId(Constants::REMOTE_COMPILER_SETTINGS_ID);
    setDisplayName(tr("Remote Compiler Configurations"));
    setCategory(Constants::REMOTE_COMPILER_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("Remote compiler", Constants::REMOTE_COMPILER_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Constants::REMOTE_COMPILER_SETTINGS_CATEGORY_ICON));
}

QWidget* RemoteCompilerSettingsPage::widget()
{
    if(!m_widget)
        m_widget = new RemoteCompilerSettingsWidget;
    return m_widget;
}

void RemoteCompilerSettingsPage::apply()
{
    m_widget->saveSettings();
}

void RemoteCompilerSettingsPage::finish()
{
    delete m_widget;
}

} //Internal
} //RemoteCompiler

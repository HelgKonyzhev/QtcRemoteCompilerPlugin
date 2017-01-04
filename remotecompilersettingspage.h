#ifndef REMOTECOMPILERSETTINGSPAGE_H
#define REMOTECOMPILERSETTINGSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>
#include <QPointer>

namespace RemoteCompiler {
namespace Internal {

class RemoteCompilerSettingsWidget;

class RemoteCompilerSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

    QPointer<RemoteCompilerSettingsWidget> m_widget;

public:
    explicit RemoteCompilerSettingsPage(QObject *parent = 0);

    QWidget *widget();
    void apply();
    void finish();
};

} //Internal
} //RemoteCompiler

#endif // REMOTECOMPILERSETTINGSPAGE_H

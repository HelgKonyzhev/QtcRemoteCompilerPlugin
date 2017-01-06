#ifndef REMOTECOMPILERSETTINGSWIDGET_H
#define REMOTECOMPILERSETTINGSWIDGET_H

#include "remotecompilerconfigurations.h"

#include <QWidget>
#include <QAbstractTableModel>
#include <QVector>
#include <QPointer>

namespace Ui {
class RemoteCompilerSettingsWidget;
}

namespace RemoteCompiler {
namespace Internal {

class CHTModel : public QAbstractTableModel
{
    Q_OBJECT

    QVector<CompilationHostInfo> m_hosts;

public:
    void setHosts(const QVector<CompilationHostInfo> &hosts);

protected:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
};


class RemoteCompilerSettingsWidget : public QWidget
{
    Q_OBJECT

    CHTModel m_CHTModel; // CH - Compilation Host; CHT - Compilation Host Table
    RemoteCompilerConfig m_config;
    int m_selectedCHIndex;

    void updateCompilationHosts(const QVector<CompilationHostInfo> &newNodes);
    void enableCHControls(bool b);
    bool assertHostCorrect(const CompilationHostInfo &host);
    void CHSelected(const QModelIndex &index);
    void editCH();

public:
    explicit RemoteCompilerSettingsWidget(QWidget *parent = 0);
    ~RemoteCompilerSettingsWidget();

    void saveSettings();

private slots:
    void on_addCHButton_clicked();
    void on_removeCHButton_clicked();
    void on_createKitsCheckBox_stateChanged(int arg1);
    void on_CHTableView_clicked(const QModelIndex &index);
    void on_editCHButton_clicked();

    void on_CHTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::RemoteCompilerSettingsWidget *m_ui;
};

} //Internal
} //RemoteCompiler

#endif // REMOTECOMPILERSETTINGSWIDGET_H

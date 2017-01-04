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

class CNTModel : public QAbstractTableModel
{
    Q_OBJECT

    QVector<CompilationNodeInfo> m_nodes;

public:
    void setNodes(const QVector<CompilationNodeInfo> &nodes);

protected:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
};


class RemoteCompilerSettingsWidget : public QWidget
{
    Q_OBJECT

    CNTModel m_CNTModel; // CN - Compilation Node; CNT - Compilation Node Table
    RemoteCompilerConfig m_config;
    int m_selectedCNIndex;

    void updateCompilationNodes(const QVector<CompilationNodeInfo> &newNodes);
    void enableCNControls(bool b);
    bool assertNodeCorrect(const CompilationNodeInfo &node);

public:
    explicit RemoteCompilerSettingsWidget(QWidget *parent = 0);
    ~RemoteCompilerSettingsWidget();

    void saveSettings();

private slots:
    void on_addCNButton_clicked();
    void on_removeCNButton_clicked();
    void on_createKitsCheckBox_stateChanged(int arg1);
    void on_CNTableView_clicked(const QModelIndex &index);
    void on_editCNButton_clicked();

private:
    Ui::RemoteCompilerSettingsWidget *m_ui;
};

} //Internal
} //RemoteCompiler

#endif // REMOTECOMPILERSETTINGSWIDGET_H

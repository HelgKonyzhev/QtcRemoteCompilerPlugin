#include "remotecompilersettingswidget.h"
#include "ui_remotecompilersettingswidget.h"
#include "addcompilationnodedialog.h"

#include <QHeaderView>

namespace RemoteCompiler {
namespace Internal {

void CNTModel::setNodes(const QVector<CompilationNodeInfo> &nodes)
{
    beginResetModel();
    m_nodes = nodes;
    endResetModel();
}

QVariant CNTModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();
    switch (index.column()) {
    case 0:
        return m_nodes[index.row()].user;
    case 1:
        return m_nodes[index.row()].server;
    case 2:
        return QString();
    }
    return QVariant();
}

QVariant CNTModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("User");
        case 1:
            return tr("Server");
        case 2:
            return tr("Status");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

int CNTModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_nodes.size();
}

int CNTModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}


RemoteCompilerSettingsWidget::RemoteCompilerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    m_config(RemoteCompilerConfigurations::currentConfig()),
    m_ui(new Ui::RemoteCompilerSettingsWidget)
{
    m_ui->setupUi(this);
    m_selectedCNIndex = -1;

    m_CNTModel.setNodes(m_config.compilationNodes());
    m_ui->createKitsCheckBox->setChecked(m_config.autoCreatingKits());

    m_ui->CNTableView->setModel(&m_CNTModel);
    m_ui->CNTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_ui->CNTableView->verticalHeader()->setVisible(false);
    m_ui->CNWarningIcon->setVisible(false);
    m_ui->CNWarningLabel->setVisible(false);
}

RemoteCompilerSettingsWidget::~RemoteCompilerSettingsWidget()
{
    delete m_ui;
}

void RemoteCompilerSettingsWidget::updateCompilationNodes(const QVector<CompilationNodeInfo> &newNodes)
{
    m_config.setCompilationNodes(newNodes);
    m_CNTModel.setNodes(newNodes);
}

void RemoteCompilerSettingsWidget::enableCNControls(bool b)
{
    m_ui->removeCNButton->setEnabled(b);
    m_ui->editCNButton->setEnabled(b);
}

bool RemoteCompilerSettingsWidget::assertNodeCorrect(const CompilationNodeInfo &node)
{
    bool ret;
    if(!(ret = !m_config.compilationNodes().contains(node))) {
        m_ui->CNWarningLabel->setText(trUtf8("Node ") +
                                      node.user + QString::fromUtf8("@") + node.server +
                                      trUtf8(" exists"));
    }
    m_ui->CNWarningIcon->setVisible(!ret);
    m_ui->CNWarningLabel->setVisible(!ret);
    return ret;
}

void RemoteCompilerSettingsWidget::saveSettings()
{
    RemoteCompilerConfigurations::setConfig(m_config);
}

void RemoteCompilerSettingsWidget::on_addCNButton_clicked()
{
    CompilationNodeInfo node;
    int res = AddCompilationNodeDialog::getNodeInfo(node);
    if(res == QDialog::Accepted) {
        QVector<CompilationNodeInfo> nodes = m_config.compilationNodes();
        if(!assertNodeCorrect(node))
            return;
        nodes.append(node);
        updateCompilationNodes(nodes);
        if(m_selectedCNIndex != -1) {
            m_selectedCNIndex = nodes.size() - 1;
            m_ui->CNTableView->selectRow(m_selectedCNIndex);
        }
    }
}

void RemoteCompilerSettingsWidget::on_removeCNButton_clicked()
{
    QVector<CompilationNodeInfo> nodes = m_config.compilationNodes();
    nodes.remove(m_selectedCNIndex);
    updateCompilationNodes(nodes);
    if(m_selectedCNIndex != -1) {
        if(nodes.size() > 0) {
            m_ui->CNTableView->selectRow(--m_selectedCNIndex);
        } else {
            m_selectedCNIndex = -1;
            enableCNControls(false);
        }
    }
}

void RemoteCompilerSettingsWidget::on_createKitsCheckBox_stateChanged(int arg1)
{
    m_config.setAutoCreatingKits(arg1 == Qt::Checked);
}

void RemoteCompilerSettingsWidget::on_CNTableView_clicked(const QModelIndex &index)
{
    m_selectedCNIndex = index.row();
    enableCNControls(true);
}

void RemoteCompilerSettingsWidget::on_editCNButton_clicked()
{
    QVector<CompilationNodeInfo> nodes = m_config.compilationNodes();
    CompilationNodeInfo editingNode = nodes[m_selectedCNIndex];
    int res = AddCompilationNodeDialog::getNodeInfo(editingNode);
    if(res == QDialog::Accepted) {
        if(!assertNodeCorrect(editingNode))
            return;
        nodes[m_selectedCNIndex] = editingNode;
        updateCompilationNodes(nodes);
        m_ui->CNTableView->selectRow(m_selectedCNIndex);
    }
}


} //Internal
} //RemoteCompiler

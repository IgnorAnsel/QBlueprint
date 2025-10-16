#include "ForLoopDebugger.h"
#include <QDebug>

ForLoopDebugger::ForLoopDebugger(QBlueprintNode* forLoopNode, QWidget* parent)
    : QDialog(parent), m_forLoopNode(forLoopNode)
{
    setWindowTitle("ForLoop Debugger");
    setFixedSize(600, 400);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // 状态标签
    m_statusLabel = new QLabel("Debugging: " + forLoopNode->getNodeTitle());
    layout->addWidget(m_statusLabel);
    
    // 调试输出
    m_debugOutput = new QTextEdit();
    m_debugOutput->setReadOnly(true);
    layout->addWidget(m_debugOutput);
    
    // 控制按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    QPushButton* refreshBtn = new QPushButton("Refresh");
    QPushButton* triggerBtn = new QPushButton("Trigger Loop");
    QPushButton* stepBtn = new QPushButton("Step");
    QPushButton* closeBtn = new QPushButton("Close");
    
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addWidget(triggerBtn);
    buttonLayout->addWidget(stepBtn);
    buttonLayout->addWidget(closeBtn);
    
    layout->addLayout(buttonLayout);
    
    // 连接信号
    connect(refreshBtn, &QPushButton::clicked, this, &ForLoopDebugger::refreshDebugInfo);
    connect(triggerBtn, &QPushButton::clicked, this, &ForLoopDebugger::triggerLoopManually);
    connect(stepBtn, &QPushButton::clicked, this, &ForLoopDebugger::stepThroughLoop);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    
    // 初始刷新
    refreshDebugInfo();
}

void ForLoopDebugger::refreshDebugInfo()
{
    m_debugOutput->clear();
    
    QString debugInfo;
    debugInfo += "=== ForLoop Node Debug Info ===\n\n";
    
    updatePortInfo();
    updateConnectionInfo();
    
    m_debugOutput->setText(debugInfo);
}

void ForLoopDebugger::updatePortInfo()
{
    QString info = "Port Information:\n";
    
    info += "Input Ports:\n";
    for (QBlueprintPort* port : m_forLoopNode->getInputPorts()) {
        info += QString("  %1: %2 (Type: %3)\n")
            .arg(port->name())
            .arg(port->data().toString())
            .arg(port->getVarTypeName());
    }
    
    info += "Output Ports:\n";
    for (QBlueprintPort* port : m_forLoopNode->getOutputPorts()) {
        info += QString("  %1: %2 (Type: %3)\n")
            .arg(port->name())
            .arg(port->data().toString())
            .arg(port->getVarTypeName());
    }
    
    m_debugOutput->append(info);
}

void ForLoopDebugger::updateConnectionInfo()
{
    QString info = "Connection Information:\n";
    
    // 获取场景和连接信息
    auto scene = m_forLoopNode->scene();
    if (scene) {
        // 这里需要根据您的连接管理逻辑来实现
        info += "Connections will be displayed here\n";
    }
    
    m_debugOutput->append(info);
}

void ForLoopDebugger::triggerLoopManually()
{
    qDebug() << "Manually triggering ForLoop...";
    
    // 触发事件输入端口
    for (QBlueprintPort* port : m_forLoopNode->getInputPorts()) {
        if (port->portType() == QBlueprintPort::EVENT_INPUT) {
            port->receiveData(QVariant::fromValue(true));
            m_debugOutput->append("Loop triggered manually");
            break;
        }
    }
    
    refreshDebugInfo();
}

void ForLoopDebugger::stepThroughLoop()
{
    m_debugOutput->append("Step function - Not implemented yet");
}
#ifndef FORLOOPDEBUGGER_H
#define FORLOOPDEBUGGER_H

#include "qblueprintnode.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>

class ForLoopDebugger : public QDialog
{
    Q_OBJECT

public:
    explicit ForLoopDebugger(QBlueprintNode* forLoopNode, QWidget* parent = nullptr);
    
private slots:
    void refreshDebugInfo();
    void triggerLoopManually();
    void stepThroughLoop();

private:
    QBlueprintNode* m_forLoopNode;
    QTextEdit* m_debugOutput;
    QLabel* m_statusLabel;
    
    void updatePortInfo();
    void updateConnectionInfo();
};

#endif // FORLOOPDEBUGGER_H
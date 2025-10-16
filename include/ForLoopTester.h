#ifndef FORLOOPTESTER_H
#define FORLOOPTESTER_H

#include "qblueprint.h"
#include "qblueprintnode.h"
#include <QObject>

class ForLoopTester : public QObject
{
    Q_OBJECT

public:
    explicit ForLoopTester(QBlueprint* blueprint, QObject* parent = nullptr);
    
    void runAllTests();
    
private slots:
    void testPortCreation();
    void testDataFlow();
    void testEventFlow();
    void testLoopExecution();
    void testNestedLoops();
    
private:
    QBlueprint* m_blueprint;
    QBlueprintNode* createTestForLoopNode();
    void cleanupTestNodes();
};

#endif // FORLOOPTESTER_H
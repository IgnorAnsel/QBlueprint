#include "ForLoopTester.h"
#include <QDebug>
#include <QTimer>
#include <QApplication>

ForLoopTester::ForLoopTester(QBlueprint* blueprint, QObject* parent)
    : QObject(parent), m_blueprint(blueprint)
{
}

void ForLoopTester::runAllTests()
{
    qDebug() << "=== Starting ForLoop Tests ===";

    testPortCreation();
    testDataFlow();
    testEventFlow();
    testLoopExecution();
    testNestedLoops();
}

void ForLoopTester::testPortCreation()
{
    qDebug() << "\n--- Test 1: Port Creation ---";

    QBlueprintNode* forLoopNode = createTestForLoopNode();

    // 更新期望值：现在有5个输入端口
    int inputCount = forLoopNode->getInputPorts().size();
    int outputCount = forLoopNode->getOutputPorts().size();

    qDebug() << "Input ports:" << inputCount << "Expected: 5 (Execute + Start + End + Step + IterationDone)";
    qDebug() << "Output ports:" << outputCount << "Expected: 3 (LoopBody + Index + Completed)";

    // 详细检查每个端口
    qDebug() << "Input Ports Details:";
    for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
        qDebug() << "  -" << port->name() << "Type:" << port->getVarTypeName()
                 << "PortType:" << port->portType();
    }

    qDebug() << "Output Ports Details:";
    for (QBlueprintPort* port : forLoopNode->getOutputPorts()) {
        qDebug() << "  -" << port->name() << "Type:" << port->getVarTypeName()
                 << "PortType:" << port->portType();
    }

    // 验证
    if (inputCount == 5 && outputCount == 3) {
        qDebug() << "✓ Port creation test PASSED";
    } else {
        qDebug() << "✗ Port creation test FAILED";
        qDebug() << "Expected 5 input ports and 3 output ports";
    }

    cleanupTestNodes();
}

void ForLoopTester::testDataFlow()
{
    qDebug() << "\n--- Test 2: Data Flow ---";

    QBlueprintNode* forLoopNode = createTestForLoopNode();

    // 测试设置和获取端口值
    forLoopNode->setPortValue("Start", 5);
    forLoopNode->setPortValue("End", 15);
    forLoopNode->setPortValue("Step", 2);

    // 验证数据是否正确设置
    QVariant startValue = forLoopNode->getPortValue("Start");
    QVariant endValue = forLoopNode->getPortValue("End");
    QVariant stepValue = forLoopNode->getPortValue("Step");
    QVariant indexValue = forLoopNode->getPortValue("Index");
    QVariant completedValue = forLoopNode->getPortValue("Completed");

    qDebug() << "Start value:" << startValue.toInt();
    qDebug() << "End value:" << endValue.toInt();
    qDebug() << "Step value:" << stepValue.toInt();
    qDebug() << "Index value:" << indexValue.toInt();
    qDebug() << "Completed value:" << completedValue.toBool();

    bool testPassed = (startValue.toInt() == 5) &&
                     (endValue.toInt() == 15) &&
                     (stepValue.toInt() == 2) &&
                     (indexValue.toInt() == 0) &&
                     (completedValue.toBool() == false);

    if (testPassed) {
        qDebug() << "✓ Data flow test PASSED";
    } else {
        qDebug() << "✗ Data flow test FAILED";
    }

    cleanupTestNodes();
}

void ForLoopTester::testEventFlow()
{
    qDebug() << "\n--- Test 3: Event Flow ---";

    QBlueprintNode* forLoopNode = createTestForLoopNode();

    // 设置循环参数
    forLoopNode->setPortValue("Start", 0);
    forLoopNode->setPortValue("End", 3);
    forLoopNode->setPortValue("Step", 1);

    qDebug() << "Testing complete loop execution...";

    // 记录初始状态
    QVariant initialIndex = forLoopNode->getPortValue("Index");
    QVariant initialCompleted = forLoopNode->getPortValue("Completed");

    qDebug() << "Before event - Index:" << initialIndex << "Completed:" << initialCompleted;

    // 手动触发事件
    bool eventTriggered = false;
    for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
        if (port->name() == "Execute") {
            qDebug() << "Triggering Execute port";
            port->receiveData(QVariant::fromValue(true));
            eventTriggered = true;
            break;
        }
    }

    if (!eventTriggered) {
        qDebug() << "✗ Execute port not found!";
        cleanupTestNodes();
        return;
    }

    // 模拟循环体执行完成反馈
    QTimer::singleShot(100, [=]() {
        qDebug() << "Simulating iteration 0 completion...";

        // 发送迭代完成信号
        for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
            if (port->name() == "IterationDone") {
                port->receiveData(QVariant::fromValue(true));
                qDebug() << "Sent iteration 0 done signal";
                break;
            }
        }
    });

    QTimer::singleShot(200, [=]() {
        qDebug() << "Simulating iteration 1 completion...";

        for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
            if (port->name() == "IterationDone") {
                port->receiveData(QVariant::fromValue(true));
                qDebug() << "Sent iteration 1 done signal";
                break;
            }
        }
    });

    QTimer::singleShot(300, [=]() {
        qDebug() << "Simulating iteration 2 completion...";

        for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
            if (port->name() == "IterationDone") {
                port->receiveData(QVariant::fromValue(true));
                qDebug() << "Sent iteration 2 done signal";
                break;
            }
        }
    });

    QTimer::singleShot(400, [=]() {
        qDebug() << "Simulating iteration 3 completion...";

        for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
            if (port->name() == "IterationDone") {
                port->receiveData(QVariant::fromValue(true));
                qDebug() << "Sent iteration 3 done signal";
                break;
            }
        }
    });

    // 最终检查
    QTimer::singleShot(500, [=]() {
        QVariant finalIndex = forLoopNode->getPortValue("Index");
        QVariant finalCompleted = forLoopNode->getPortValue("Completed");

        qDebug() << "After loop - Index:" << finalIndex << "Completed:" << finalCompleted;

        if (finalIndex.toInt() == 3 && finalCompleted.toBool()) {
            qDebug() << "✓ Event flow test PASSED - Loop completed successfully";
        } else {
            qDebug() << "✗ Event flow test FAILED - Loop did not complete correctly";
            qDebug() << "Expected Index: 3, Actual:" << finalIndex.toInt();
            qDebug() << "Expected Completed: true, Actual:" << finalCompleted.toBool();
        }

        cleanupTestNodes();
    });
}

void ForLoopTester::testLoopExecution()
{
    qDebug() << "\n--- Test 4: Loop Execution ---";

    QBlueprintNode* forLoopNode = createTestForLoopNode();

    // 设置循环参数
    forLoopNode->setPortValue("Start", 0);
    forLoopNode->setPortValue("End", 3);
    forLoopNode->setPortValue("Step", 1);

    qDebug() << "Starting loop execution test...";
    qDebug() << "Expected: Index should go from 0 to 3";

    // 由于没有dataChanged信号，我们使用定时器轮询
    int checkCount = 0;
    QVariant lastIndex = forLoopNode->getPortValue("Index");

    QTimer* checkTimer = new QTimer(this);
    connect(checkTimer, &QTimer::timeout, [=, &checkCount, &lastIndex]() mutable {
        QVariant currentIndex = forLoopNode->getPortValue("Index");
        QVariant completed = forLoopNode->getPortValue("Completed");

        if (currentIndex != lastIndex) {
            qDebug() << "Iteration" << checkCount << "- Index changed from"
                     << lastIndex << "to" << currentIndex;
            lastIndex = currentIndex;
        }

        checkCount++;

        if (completed.toBool() || checkCount >= 10) { // 最多检查10次
            qDebug() << "Loop check completed after" << checkCount << "checks";
            qDebug() << "Final Index:" << currentIndex << "Completed:" << completed;

            if (currentIndex.toInt() == 3 && completed.toBool()) {
                qDebug() << "✓ Loop execution test PASSED";
            } else {
                qDebug() << "✗ Loop execution test FAILED";
            }

            checkTimer->stop();
            checkTimer->deleteLater();
            cleanupTestNodes();
        }
    });

    // 触发循环开始
    bool eventFound = false;
    for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
        if (port->portType() == QBlueprintPort::EVENT_INPUT) {
            port->receiveData(QVariant::fromValue(true));
            eventFound = true;
            qDebug() << "Loop triggered, starting monitoring...";
            break;
        }
    }

    if (eventFound) {
        checkTimer->start(200); // 每200ms检查一次
    } else {
        qDebug() << "✗ No event port found to trigger loop";
        cleanupTestNodes();
    }
}

void ForLoopTester::testNestedLoops()
{
    qDebug() << "\n--- Test 5: Nested Loops ---";

    // 创建两个ForLoop节点测试嵌套
    QBlueprintNode* outerLoop = createTestForLoopNode();
    QBlueprintNode* innerLoop = createTestForLoopNode();

    outerLoop->setPortValue("Start", 0);
    outerLoop->setPortValue("End", 2);
    outerLoop->setPortValue("Step", 1);

    innerLoop->setPortValue("Start", 10);
    innerLoop->setPortValue("End", 12);
    innerLoop->setPortValue("Step", 1);

    qDebug() << "Nested loops setup: Outer(0-2), Inner(10-12)";
    qDebug() << "Manual verification required for nested loops";
    qDebug() << "✓ Nested loops test READY";

    cleanupTestNodes();
}

QBlueprintNode* ForLoopTester::createTestForLoopNode()
{
    QBlueprintNode* node = new QBlueprintNode(Type::FORLOOP);
    node->setNodeTitle("TestForLoop");
    node->setClassName("Control");
    node->addInputPort(Type::FORLOOP);
    node->addOutputPort(Type::FORLOOP);
    // 添加到场景
    if (m_blueprint && m_blueprint->getScene()) {
        m_blueprint->getScene()->addItem(node);
        node->setPos(100, 100);
    }

    return node;
}

void ForLoopTester::cleanupTestNodes()
{
    qDebug() << "Cleaning up test nodes (no scene items access)...";


}
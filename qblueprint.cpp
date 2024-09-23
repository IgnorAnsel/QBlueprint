#include "qblueprint.h"
#include "ui_qblueprint.h"
#include <unordered_map>

QBlueprint::QBlueprint(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this))  // 初始化场景
{
    // 设置场景的范围，可以根据需要调整
    scene->setSceneRect(0, 0, 2000, 2000);

    // 将场景设置为QGraphicsView的场景
    setScene(scene);

    // 可选：设置视图的一些属性，比如抗锯齿等
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);  // 设置拖拽模式
    // 初始视图缩放
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setBackgroundBrush(QColor(30, 30, 30));  // 设置深色背景
    createBlueprintNodes();

}

void QBlueprint::createBlueprintNodes() // 使用工厂方法基于函数生成节点
{

    createOutputNode();
    createInputNode();
    QBlueprintNode* testclass_add_node = QNodeFactory::createNodeFromFunction(this, &TestClass::add,"add","TestClass"); // 自动获取函数名不正常，直接填写你需要的名称
    QBlueprintNode* testclass_test_node = QNodeFactory::createNodeFromFunction(this, &TestClass::test,"test", "TestClass");
    QBlueprintNode* qblueprint_add_node = QNodeFactory::createNodeFromFunction(this, &add,"add");
    QBlueprintNode* qblueprint_deletea_node = QNodeFactory::createNodeFromFunction(this, &deletea,"deletea");


    // 创建 Math 相关的运算节点
    QBlueprintNode* math_add_node = QNodeFactory::createNodeFromFunction(this, &Math::add, "add", "Math");
    QBlueprintNode* math_subtract_node = QNodeFactory::createNodeFromFunction(this, &Math::subtract, "subtract", "Math");
    QBlueprintNode* math_multiply_node = QNodeFactory::createNodeFromFunction(this, &Math::multiply, "multiply", "Math");
    QBlueprintNode* math_divide_node = QNodeFactory::createNodeFromFunction(this, &Math::divide, "divide", "Math");
    QBlueprintNode* math_sqrt_node = QNodeFactory::createNodeFromFunction(this, &Math::sqrt, "sqrt", "Math");
    QBlueprintNode* math_pow_node = QNodeFactory::createNodeFromFunction(this, &Math::pow, "pow", "Math");
    classifyNodes();
}

void QBlueprint::classifyNodes()
{
    std::unordered_map<QString, std::vector<QString>> classifiedNodes;
    for (QBlueprintNode* node : save_nodes) {
        QString className = node->getClassName();
        QString functionName = node->getNodeTitle();

        // 将函数名添加到对应类名的列表中
        classifiedNodes[className].push_back(functionName);
    }

    // 输出分类结果，或者进一步处理
    for (const auto& entry : classifiedNodes) {
        QString className = entry.first;
        const std::vector<QString>& functions = entry.second;

        qDebug() << "Class Name:" << className;
        for (const QString& func : functions) {
            qDebug() << "  Function:" << func;
        }
    }
}

int QBlueprint::add(int a, int b, int c)
{
    return a + b + c;
}
int QBlueprint::deletea(int a)
{
    return a;
}
QBlueprint::~QBlueprint()
{

}
void QBlueprint::contextMenuEvent(QContextMenuEvent* event)
{
    // 创建右键菜单（主菜单，一级菜单）
    QMenu contextMenu;
    QPointF scenePos = mapToScene(event->pos());

    // 设置 contextMenu（一级菜单）的样式表，保留圆角
    contextMenu.setStyleSheet(
        "QMenu { "
        "   background-color: #353535; "  // 一级菜单背景色
        "   color: white; "               // 一级菜单文字颜色
        "   border: 2px solid #5A5A5A; "  // 一级菜单边框
        "   border-radius: 8px; "         // 圆角边框
        "} "
        "QMenu::item:selected { "
        "   background-color: #2A82DA; "  // 鼠标悬停时的背景色（一级菜单）
        "   color: white; "               // 鼠标悬停时的文字颜色
        "   border-radius: 4px; "          // 圆角边框
        "}"
        );

    // 使用一个 map 来临时存储类名和对应的节点列表
    QMap<QString, QList<QBlueprintNode*>> classNodeMap;
    for (QBlueprintNode* node : save_nodes) {
        QString className = node->getClassName();
        classNodeMap[className].append(node);
    }

    // 遍历每个类
    for (auto it = classNodeMap.begin(); it != classNodeMap.end(); ++it) {
        QString className = it.key();
        QList<QBlueprintNode*> nodes = it.value();

        // 创建类名的一级菜单（作为 contextMenu 的子菜单）
        QMenu* classMenu = contextMenu.addMenu(className);

        // 根据类名设置不同的样式表，并保留圆角
        if (className == "Input") {
            // 设置 Input 类的样式：绿色（二级菜单），带圆角
            classMenu->setStyleSheet(
                "QMenu { "
                "   background-color: #DFFFD6; "  // 二级菜单的绿色背景
                "   color: black; "               // 二级菜单的文字颜色
                "   border: 2px solid green; "     // 二级菜单的绿色边框
                "   border-radius: 8px; "          // 圆角边框
                "} "
                "QMenu::item:selected { "
                "   background-color: #8CD98C; "  // 鼠标悬停时的绿色背景
                "   color: black; "               // 鼠标悬停时的文字颜色
                "   border-radius: 4px; "          // 圆角边框
                "}"
                );
        } else if (className == "Output") {
            // 设置 Output 类的样式：红色（二级菜单），带圆角
            classMenu->setStyleSheet(
                "QMenu { "
                "   background-color: #FFD6D6; "  // 二级菜单的红色背景
                "   color: black; "               // 二级菜单的文字颜色
                "   border: 2px solid red; "       // 二级菜单的红色边框
                "   border-radius: 8px; "          // 圆角边框
                "} "
                "QMenu::item:selected { "
                "   background-color: #FF7A7A; "  // 鼠标悬停时的红色背景
                "   color: black; "               // 鼠标悬停时的文字颜色
                "   border-radius: 4px; "          // 圆角边框
                "}"
                );
        } else {
            // 设置其他类的样式：蓝色（二级菜单），带圆角
            classMenu->setStyleSheet(
                "QMenu { "
                "   background-color: #D6E6FF; "  // 二级菜单的蓝色背景
                "   color: black; "               // 二级菜单的文字颜色
                "   border: 2px solid blue; "      // 二级菜单的蓝色边框
                "   border-radius: 8px; "          // 圆角边框
                "} "
                "QMenu::item:selected { "
                "   background-color: #7A9EFF; "  // 鼠标悬停时的蓝色背景
                "   color: black; "               // 鼠标悬停时的文字颜色
                "   border-radius: 4px; "          // 圆角边框
                "}"
                );
        }

        // 添加该类的所有函数到二级菜单中
        for (QBlueprintNode* node : nodes) {
            QString functionName = node->getNodeTitle();
            QAction* action = classMenu->addAction(functionName);

            // 使用 lambda 表达式捕获节点信息
            connect(action, &QAction::triggered, [this, node, scenePos]() {
                placeNodeInScene(node, scenePos);
            });
        }
    }

    // 显示菜单
    contextMenu.exec(event->globalPos());
}


void QBlueprint::placeNodeInScene(QBlueprintNode* originalNode, const QPointF& mousePos)
{
    // 使用 clone 方法创建节点的副本
    QBlueprintNode* newNode = originalNode->clone();
    // 设置位置
    newNode->setPos(mousePos);

    // 将节点添加到场景
    scene->addItem(newNode);

    // 将节点添加到 scene_nodes 向量中
    scene_nodes.push_back(newNode);
}


void QBlueprint::pushVectorQBlueprintNode(QBlueprintNode *node)
{
    save_nodes.push_back(node);
}

// 重载绘制背景方法
void QBlueprint::drawBackground(QPainter *painter, const QRectF &rect)
{
    // 调用父类的绘制背景方法（以便保持默认行为）
    QGraphicsView::drawBackground(painter, rect);

    // 绘制网格背景
    const int gridSize = 20;  // 网格大小
    QPen lightPen(QColor(60, 60, 60), 1);  // 浅色网格线
    QPen darkPen(QColor(25, 25, 25), 2);   // 深色网格线

    // 获取当前视图的矩形区域
    QRectF viewRect = rect;

    // 绘制小网格线
    painter->setPen(lightPen);
    for (qreal x = std::floor(viewRect.left() / gridSize) * gridSize; x < viewRect.right(); x += gridSize)
    {
        painter->drawLine(QLineF(x, viewRect.top(), x, viewRect.bottom()));
    }
    for (qreal y = std::floor(viewRect.top() / gridSize) * gridSize; y < viewRect.bottom(); y += gridSize)
    {
        painter->drawLine(QLineF(viewRect.left(), y, viewRect.right(), y));
    }

    // 绘制大网格线（每隔5个小网格）
    painter->setPen(darkPen);
    for (qreal x = std::floor(viewRect.left() / (gridSize * 5)) * (gridSize * 5); x < viewRect.right(); x += gridSize * 5)
    {
        painter->drawLine(QLineF(x, viewRect.top(), x, viewRect.bottom()));
    }
    for (qreal y = std::floor(viewRect.top() / (gridSize * 5)) * (gridSize * 5); y < viewRect.bottom(); y += gridSize * 5)
    {
        painter->drawLine(QLineF(viewRect.left(), y, viewRect.right(), y));
    }
}

void QBlueprint::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;  // 缩放因子

    // 获取当前视图的变换矩阵
    QTransform currentTransform = transform();

    // 当前的缩放比例，假设 x 和 y 的缩放比例相同
    double currentScale = currentTransform.m11();  // m11() 获取水平缩放比例

    // 根据滚轮滚动方向进行缩放，并限制缩放范围
    if (event->angleDelta().y() > 0)  // 如果滚轮向上滚动，试图放大
    {
        if (currentScale < maxScaleFactor)  // 限制最大缩放
        {
            scale(scaleFactor, scaleFactor);  // 放大视图
        }
    }
    else  // 如果滚轮向下滚动，试图缩小
    {
        if (currentScale > minScaleFactor)  // 限制最小缩放
        {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // 缩小视图
        }
    }

    // 获取鼠标在视图中的位置
    QPointF viewPos = event->position();

    // 将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(viewPos.toPoint());

    // 获取缩放后的鼠标位置在场景中的位置
    QPointF newScenePos = mapToScene(viewPos.toPoint());

    // 计算视图需要移动的距离，以使得缩放后鼠标位置不变
    QPointF delta = newScenePos - scenePos;

    // 调整视图的中心，使得缩放后的鼠标位置保持不变
    setTransformationAnchor(QGraphicsView::NoAnchor);
    translate(delta.x(), delta.y());
    // 在缩放后更新所有连接的位置
    updateAllConnections();
    event->accept();  // 接受事件
}
void QBlueprint::mousePressEvent(QMouseEvent *event)
{
    // 将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(event->pos());

    // 查找鼠标点击的图元
    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
    qDebug()<< "clicked item:" << item;
    // 首先检查是否点击在端口上
    QBlueprintPort *port = dynamic_cast<QBlueprintPort*>(item);
    if (port)
    {
        qDebug() << "Clicked on port:" << port->name();
        m_draggingPort = port;
        startConnectionDrag(scenePos);
        return;
    }

    // 然后检查是否点击在节点上
    QBlueprintNode *node = dynamic_cast<QBlueprintNode*>(item);
    if (node)
    {
        // 使用访问器方法获取端口列表
        const auto& inputPorts = node->getInputPorts();
        const auto& outputPorts = node->getOutputPorts();

        // 遍历找到点击的端口
        for (auto *port : inputPorts)
        {
            if (port->contains(port->mapFromScene(scenePos)))
            {
                qDebug() << "Clicked on input port:" << port->name();
                m_draggingPort = port;
                startConnectionDrag(scenePos);
                return;
            }
        }

        for (auto *port : outputPorts)
        {
            if (port->contains(port->mapFromScene(scenePos)))
            {
                qDebug() << "Clicked on output port:" << port->name();
                m_draggingPort = port;
                startConnectionDrag(scenePos);
                return;
            }
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void QBlueprint::mouseMoveEvent(QMouseEvent *event)
{
    if (m_draggingPort && m_currentConnection)
    {
        // 将视图坐标转换为场景坐标
        QPointF scenePos = mapToScene(event->pos());

        // 更新临时连线的位置
        m_currentConnection->updatePosition(m_draggingPort->centerPos(), scenePos);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void QBlueprint::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_draggingPort && m_currentConnection)
    {
        // 将视图坐标转换为场景坐标
        QPointF scenePos = mapToScene(event->pos());

        // 遍历场景中的所有项目，寻找匹配的端口
        QBlueprintPort *targetPort = nullptr;
        for (QGraphicsItem *item : scene->items(scenePos))
        {
            targetPort = dynamic_cast<QBlueprintPort*>(item);
            if (targetPort && targetPort != m_draggingPort && targetPort->portType() != m_draggingPort->portType()&&targetPort->parentItem() != m_draggingPort->parentItem())
            {
                break;  // 找到目标端口，退出循环
            }
        }
        if(targetPort)
        {
            if((targetPort->portType() == QBlueprintPort::EVENT_INPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_OUTPUT)
                ||(targetPort->portType() == QBlueprintPort::EVENT_OUTPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_INPUT))
            {
                qDebug() << "事件端口连接";
                m_currentConnection->setEndPort(targetPort);
                m_draggingPort->sendDataToConnectedPorts();
                propagateDataFromInitialNode(m_currentConnection->startPort());
            }
            else if ((m_currentConnection->startPort()->getVarTypeName()==targetPort->getVarTypeName())
                     && targetPort->portType()!=QBlueprintPort::EVENT_INPUT && targetPort->portType()!=QBlueprintPort::EVENT_OUTPUT)
            {
                qDebug() << "Found target port:" << targetPort->name();
                // 连接两个端口
                m_currentConnection->setEndPort(targetPort);
                m_draggingPort->sendDataToConnectedPorts();
            }
            else if(m_currentConnection->startPort()->getVarTypeName()==targetPort->getVarTypeName())
                qDebug() << "真的吗";
            else
            {
                qDebug() << m_currentConnection->startPort()->getVarTypeName() << " vs " << targetPort->getVarTypeName();
                removeConnection(m_currentConnection); // 删除连接
            }
        }
        else
            removeConnection(m_currentConnection);


        m_currentConnection = nullptr;
        m_draggingPort = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void QBlueprint::updateConnectionsForPort(QBlueprintPort *port)
{
    // 遍历所有连接并更新与指定端口相关的连接
    for (QBlueprintConnection *connection : connections)
    {
        if (connection->startPort() == port)
        {
            connection->updatePosition(port->centerPos(), connection->endPort()->centerPos());
        }
        else if (connection->endPort() == port)
        {
            connection->updatePosition(connection->startPort()->centerPos(), port->centerPos());
        }
    }
}

void QBlueprint::updateAllConnections()
{
    for (QBlueprintConnection* connection : connections)
    {
        if (connection->startPort() && connection->endPort())
        {
            connection->updatePosition(connection->startPort()->centerPos(), connection->endPort()->centerPos());
        }
    }
}


// 添加连接到列表中
void QBlueprint::addConnection(QBlueprintConnection* connection)
{
    connections.push_back(connection);
    scene->addItem(connection); // 将连接添加到场景中
}

// 从列表中移除连接
void QBlueprint::removeConnection(QBlueprintConnection* connection)
{
    connections.erase(std::remove(connections.begin(), connections.end(), connection), connections.end());
    scene->removeItem(connection); // 从场景中移除连接
    delete connection;
}

// 在 startConnectionDrag 中使用 addConnection
void QBlueprint::startConnectionDrag(const QPointF &startPos)
{
    // 创建临时连线
    m_currentConnection = new QBlueprintConnection(m_draggingPort, nullptr);
    addConnection(m_currentConnection); // 将连接添加到管理列表中

    // 设置初始位置
    m_currentConnection->updatePosition(m_draggingPort->centerPos(), startPos);

    // 强制刷新场景
    scene->update();
}

void QBlueprint::createOutputNode()
{
    for (int i = 1; i < NUM_DATA_TYPES; ++i) {
        addOutputNode(static_cast<DataType>(i));
    }
}

void QBlueprint::createInputNode()
{
    for (int i = 1; i < NUM_DATA_TYPES; ++i) {
        addInputNode(static_cast<DataType>(i));
    }
}
void QBlueprint::addOutputNode(DataType dataType)
{
    QBlueprintNode* node = new QBlueprintNode(Type::OUTPUT,dataType);
    node->setClassName("Output");
    node->setNodeTitle(getEnumName(dataType));
    save_nodes.push_back(node);
}
void QBlueprint::addInputNode(DataType dataType)
{
    QBlueprintNode* node = new QBlueprintNode(Type::INPUT,dataType);
    node->setClassName("Input");
    node->setNodeTitle(getEnumName(dataType));
    save_nodes.push_back(node);
}
bool QBlueprint::isEventPortConnected(QBlueprintPort* outputPort, QBlueprintPort* inputPort) const {
    for (QBlueprintConnection* connection : connections) {
        qDebug() << "connection->startPort():" << connection->startPort()->portType() << "connection->endPort():" << connection->endPort()->portType();
        if ((/*connection->startPort() == outputPort && connection->endPort() == inputPort &&*/
             connection->startPort()->portType() == QBlueprintPort::EVENT_OUTPUT &&
             connection->endPort()->portType() == QBlueprintPort::EVENT_INPUT) ||
            (/*connection->startPort() == inputPort && connection->endPort() == outputPort &&*/
             connection->startPort()->portType() == QBlueprintPort::EVENT_INPUT &&
             connection->endPort()->portType() == QBlueprintPort::EVENT_OUTPUT)) {
            return true;
        }
    }
    return false;
}
void QBlueprint::propagateDataFromInitialNode(QBlueprintPort* initialPort)
{
    if (!initialPort) return;

    // 获取初始端口的父节点
    QBlueprintNode* initialNode = dynamic_cast<QBlueprintNode*>(initialPort->parentItem());
    if (!initialNode) return;

    // 找到该节点的所有输出端口，并发送数据
    for (QBlueprintPort* outputPort : initialNode->getOutputPorts())
    {
        outputPort->sendDataToConnectedPorts();
    }
}



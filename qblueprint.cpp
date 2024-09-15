#include "qblueprint.h"
#include "ui_qblueprint.h"

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

    // 创建并添加节点到场景中
    QBlueprintNode *node = new QBlueprintNode();
    node->setPos(10, 10);  // 设置初始位置为 (100, 100)
    scene->addItem(node);

    centerOn(node);  // 聚焦到节点
    QBlueprintNode *node2 = new QBlueprintNode();
    node2->addInputPort("412");
    node2->setPos(1000, 1000);  // 设置初始位置为 (100, 100)
    scene->addItem(node2);

}

QBlueprint::~QBlueprint()
{

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
        qDebug() << "ok";
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
            if (targetPort && targetPort != m_draggingPort && targetPort->portType() != m_draggingPort->portType())
            {
                break;  // 找到目标端口，退出循环
            }
        }

        if (targetPort)
        {
            qDebug() << "Found target port:" << targetPort->name();
            // 连接两个端口
            m_currentConnection->setEndPort(targetPort);
        }
        else
        {
            removeConnection(m_currentConnection); // 删除连接
        }

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

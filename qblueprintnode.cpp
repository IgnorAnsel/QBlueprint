#include "qblueprintnode.h"

QBlueprintNode::QBlueprintNode(QGraphicsItem *parent)
    : QGraphicsItem(parent),m_draggingPort(nullptr), m_currentConnection(nullptr)
{
    // 启用拖动（节点可以被鼠标拖动）
    setFlag(QGraphicsItem::ItemIsMovable);

    // 启用选择（节点可以被点击选中）
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

    // 初始化节点时，添加两个输入和两个输出端口
    addInputPort("Input");
    addInputPort("Input2");
    addInputPort("Input3");
    addOutputPort("Output2341");
    addOutputPort("Output2");

}
QBlueprintNode::~QBlueprintNode()
{
    if (m_currentConnection)
    {
        delete m_currentConnection;
        m_currentConnection = nullptr;
    }
}

QRectF QBlueprintNode::boundingRect() const
{
    QFont font;
    QFontMetrics fontMetrics(font);
    // 获取输入端口中最长的名称宽度
    int maxInputWidth = 0;
    for (const auto& port : inputPorts)
    {
        int textWidth = fontMetrics.horizontalAdvance(port->name());
        if (textWidth > maxInputWidth)
        {
            maxInputWidth = textWidth;
        }
    }

    // 获取输出端口中最长的名称宽度
    int maxOutputWidth = 0;
    for (const auto& port : outputPorts)
    {
        int textWidth = fontMetrics.horizontalAdvance(port->name());
        if (textWidth > maxOutputWidth)
        {
            maxOutputWidth = textWidth;
        }
    }

    // 确保节点宽度适应输入和输出端口名称的最大宽度
    int nodeWidth = maxInputWidth + maxOutputWidth + 80; // 80是节点内部的空隙
    int nodeHeight = std::max(inputPorts.size(), outputPorts.size()) * 35 + 30; // 高度根据端口数量调整

    return QRectF(0, 0, nodeWidth, nodeHeight);
}

void QBlueprintNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 设置节点的背景为圆角矩形
    QRectF rect = boundingRect();
    painter->setBrush(Qt::gray);
    painter->setPen(Qt::black);
    painter->drawRoundedRect(rect, 10, 10);  // 绘制圆角矩形，10像素圆角

    // 设置字体大小并绘制标题
    QFont font = painter->font();
    font.setPointSize(10);  // 设置字体大小
    painter->setFont(font);
    painter->setPen(Qt::black);

    // 设置标题区域
    QRectF titleRect = QRectF(rect.left(), rect.top(), rect.width(), 30);  // 标题区域高度为30
    painter->drawText(titleRect, Qt::AlignCenter, "Node");  // 居中绘制标题

    // 在标题和端口区域之间绘制一条分割线
    painter->setPen(QPen(Qt::black, 1));  // 设置线条颜色和宽度
    painter->drawLine(rect.left(), titleRect.bottom(), rect.right(), titleRect.bottom());

    // 绘制端口区域，端口从下方开始排列
    for (size_t i = 0; i < inputPorts.size(); ++i)
    {
        // 将输入端口放在左侧，调整端口位置
        inputPorts[i]->setPos(-inputPorts[i]->boundingRect().width() + 15, i * 30 + 40);  // 输入端口区域向下偏移40，避免标题
    }

    for (size_t i = 0; i < outputPorts.size(); ++i)
    {
        // 将输出端口放在右侧，调整端口位置
        outputPorts[i]->setPos(boundingRect().width() - 15, i * 30 + 40);  // 输出端口区域向下偏移40
    }
}


void QBlueprintNode::addInputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, "NULL", this);
    inputPorts.push_back(port);
}

void QBlueprintNode::addOutputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "NULL", this);
    outputPorts.push_back(port);
}

void QBlueprintNode::addInputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, name, this);
    port->setZValue(1);  // 设置 Z 值
    inputPorts.push_back(port);
}

void QBlueprintNode::addOutputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, name, this);
    port->setZValue(1);  // 设置 Z 值
    outputPorts.push_back(port);
}

//void QBlueprintNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    // 可以在此实现鼠标点击事件
//    QGraphicsItem::mousePressEvent(event);
//}

//void QBlueprintNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
//{
//    // 实现节点拖动逻辑
//    QGraphicsItem::mouseMoveEvent(event);
//}

//void QBlueprintNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    // 鼠标释放事件
//    QGraphicsItem::mouseReleaseEvent(event);
//}
void QBlueprintNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 检查是否点击了端口

    QGraphicsItem::mousePressEvent(event);

    // 如果点击在某个端口上
    for (auto *port : inputPorts)
    {
        if (port->contains(port->mapFromItem(this, event->pos())))
        {
            qDebug() << "Clicked on input port:" << port->name();
            m_draggingPort = port;
            startConnectionDrag(event->scenePos());
            return;
        }
    }

    for (auto *port : outputPorts)
    {
        if (port->contains(port->mapFromItem(this, event->pos())))
        {
            qDebug() << "Clicked on output port:" << port->name();
            m_draggingPort = port;
            startConnectionDrag(event->scenePos());
            return;
        }
    }
}


void QBlueprintNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    if (m_draggingPort && m_currentConnection)
    {
        // 更新临时连线的位置
        m_currentConnection->updatePosition(m_draggingPort->centerPos(), event->scenePos());
    }
}

void QBlueprintNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    qDebug() << "Release position:" << event->scenePos();

    // 释放连接时
    if (m_draggingPort && m_currentConnection)
    {
        // 获取鼠标释放位置的图元
        QGraphicsItem *targetItem = scene()->itemAt(event->scenePos(), QTransform());

        // 遍历找到 QBlueprintPort
        QBlueprintPort *targetPort = nullptr;
        while (targetItem) {
            targetPort = dynamic_cast<QBlueprintPort *>(targetItem);
            if (targetPort) break;
            targetItem = targetItem->parentItem();
        }
        if (targetPort) {
            qDebug() << "Detected port on release:" << targetPort->name();
        } else {
            qDebug() << "No valid port detected on release.";
        }
        if (targetPort && targetPort != m_draggingPort && targetPort->portType() != m_draggingPort->portType())
        {
            // 连接两个端口
            m_currentConnection->setEndPort(targetPort);
        }
        else
        {
            // 如果没有有效的端口，则取消连接
            delete m_currentConnection;
        }

        m_currentConnection = nullptr;
        m_draggingPort = nullptr;
    }
}


void QBlueprintNode::startConnectionDrag(const QPointF &startPos)
{
    // 创建临时连线
    m_currentConnection = new QBlueprintConnection(m_draggingPort, nullptr);
    scene()->addItem(m_currentConnection); // 将连接添加到场景中

    // 设置初始位置
    m_currentConnection->updatePosition(m_draggingPort->centerPos(), startPos);

    // 强制刷新场景
    scene()->update();
}


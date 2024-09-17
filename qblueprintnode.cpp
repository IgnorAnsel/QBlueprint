#include "qblueprintnode.h"

QBlueprintNode::QBlueprintNode(enum Type Type, DataType datatype, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    // 启用拖动（节点可以被鼠标拖动）
    setFlag(QGraphicsItem::ItemIsMovable);

    // 启用选择（节点可以被点击选中）
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);

    setZValue(1);
    setNodeType(Type);
}
QBlueprintNode::~QBlueprintNode()
{
    // 删除节点时，移除所有连接
    for (QBlueprintPort *port : inputPorts)
    {
        port->removeConnections();
    }
    for (QBlueprintPort *port : outputPorts)
    {
        port->removeConnections();
    }
}
QBlueprintNode* QBlueprintNode::clone() const
{
    // 创建一个新的 QBlueprintNode 实例
    QBlueprintNode* newNode = new QBlueprintNode(this->nodeType);
    newNode->setNodeTitle(this->m_name);
    newNode->setClassName(this->class_name);
    newNode->setNodeType(this->nodeType);
    // 克隆输入端口
    for (QBlueprintPort* port : this->inputPorts) {
        QBlueprintPort* clonedPort = port->clone(); // 假设 QBlueprintPort 有一个 clone 方法
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->inputPorts.push_back(clonedPort);
    }

    // 克隆输出端口
    for (QBlueprintPort* port : this->outputPorts) {
        QBlueprintPort* clonedPort = port->clone(); // 假设 QBlueprintPort 有一个 clone 方法
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->outputPorts.push_back(clonedPort);
    }

    // 设置克隆节点的初始位置
    newNode->setPos(this->pos());

    return newNode;
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

    // 绘制内边框
    QRectF innerRect = rect.adjusted(2, 2, -2, -2);  // 调整rect以绘制一个稍小的矩形，形成内边框效果
    painter->setBrush(Qt::NoBrush);  // 内边框不填充
    // 根据 nodeType 设置不同的内边框颜色
    if (nodeType == Type::FUNCTION)
        painter->setPen(QPen(QColor(0, 128, 255), 2));  // 设置内边框颜色为蓝色，宽度为2像素
    else if (nodeType == Type::INPUT)
        painter->setPen(QPen(QColor(0, 255, 0), 2));  // 设置内边框颜色为绿色，宽度为2像素
    else if (nodeType == Type::OUTPUT)
        painter->setPen(QPen(QColor(255, 0, 0), 2));  // 设置内边框颜色为红色，宽度为2像素
    painter->drawRoundedRect(innerRect, 8, 8);  // 绘制内边框，圆角稍微小一点

    // 设置字体大小并绘制标题
    QFont font = painter->font();
    font.setPointSize(10);  // 设置字体大小
    painter->setFont(font);
    painter->setPen(Qt::black);

    // 设置标题区域
    QRectF titleRect = QRectF(rect.left(), rect.top(), rect.width(), 30);  // 标题区域高度为30
    painter->drawText(titleRect, Qt::AlignCenter, m_name);  // 居中绘制标题

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
    port->setNodeType(nodeType);
    inputPorts.push_back(port);
}

void QBlueprintNode::addOutputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "NULL", this);
    port->setNodeType(nodeType);
    outputPorts.push_back(port);
}

void QBlueprintNode::addInputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, name, this);
    port->setNodeType(nodeType);
    qDebug() << "input" << nodeType << port->getNodeType();
    inputPorts.push_back(port);
}

void QBlueprintNode::addOutputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, name, this);
    port->setNodeType(nodeType);
    outputPorts.push_back(port);
}

void QBlueprintNode::setNodeTitle(QString name)
{
    m_name = name;
}

void QBlueprintNode::setClassName(QString class_name)
{
    QBlueprintNode::class_name = class_name;
}

QVariant QBlueprintNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        // 通知所有连接线进行更新
        for (QBlueprintPort *port : inputPorts)
        {
            port->updateConnections();
        }
        for (QBlueprintPort *port : outputPorts)
        {
            port->updateConnections();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

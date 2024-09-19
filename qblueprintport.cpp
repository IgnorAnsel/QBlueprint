#include "qblueprintport.h"
#include "qblueprintconnection.h"
#include "qblueprint.h"
QBlueprintPort::QBlueprintPort(PortType type, const QString &name, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type), m_name(name),m_font(QFont("Arial", 10))
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(2);
}
QBlueprintPort* QBlueprintPort::clone() const
{
    // 创建一个新的 QBlueprintPort 实例并复制所需的属性
    QBlueprintPort* newPort = new QBlueprintPort(this->m_type, this->m_name, nullptr); // 注意这里 parent 设为 nullptr
    newPort->setNodeType(this->parentnodeType);
    // 复制属性
    return newPort;
}
QRectF QBlueprintPort::boundingRect() const
{
    //return QRectF(-10, -10, 20, 20);  // 扩大范围以便检测
    return QRectF(0, 0, 10, 10);  // 端口的大小为 10x10
    //qDebug() << "QBlueprintPort boundingRect:" << rect;
}

void QBlueprintPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 绘制端口（不同类型的端口使用不同的颜色）
    painter->setBrush((m_type == Input) ? Qt::blue : Qt::green);  // 输入端口为蓝色，输出端口为绿色
    painter->drawEllipse(boundingRect());  // 绘制圆形端口


    painter->setFont(m_font);  // 使用成员变量中的字体

    // 获取字体的度量信息，用来计算文本宽度
    QFontMetrics fontMetrics(m_font);

    // 动态计算端口名称的宽度
    int textWidth = fontMetrics.horizontalAdvance(m_name);
    int textHeight = fontMetrics.height();
    // 绘制端口名称，放在端口旁边
    painter->setPen(Qt::white);

    if (m_type == Input)
    {
        // 输入端口：名称在端口的右边，左对齐
        QRectF textRect = boundingRect().translated(boundingRect().width() + 10, 0);  // 向右移动文本区域，留出10像素的间距
        textRect.setWidth(textWidth);  // 设置文本区域的宽度，确保名称的最后字符靠近端口
        textRect.setHeight(textHeight);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_name);  // 名称放在右侧，左对齐
    }
    else
    {
        // 输出端口：名称在端口的左边，右对齐
        QRectF textRect = boundingRect().translated(-textWidth - 10, 0);  // 根据文本长度动态调整区域，留出10像素的间距
        textRect.setWidth(textWidth);  // 设置文本区域的宽度，确保名称的最后字符靠近端口
        textRect.setHeight(textHeight);
        painter->drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, m_name);  // 名称放在左侧，右对齐
    }
}

QPointF QBlueprintPort::centerPos() const
{
    return mapToScene(boundingRect().center());  // 返回端口的中心点
}

void QBlueprintPort::updateConnections()
{
    // 检查当前场景是否存在
    QGraphicsScene *currentScene = this->scene();
    if (!currentScene) return;  // 如果场景不存在，直接返回

    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView)
    {
        // 更新该端口相关的所有连接
        blueprintView->updateConnectionsForPort(this);
    }
}

// 移除并删除该端口相关的所有连接
void QBlueprintPort::removeConnections()
{
    QGraphicsScene *currentScene = this->scene();
    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView)
    {
        std::vector<QBlueprintConnection*> toRemove;
        // 遍历所有连接，收集与此端口相关的连接
        for (QBlueprintConnection *connection : blueprintView->connections)
        {
            if (connection->startPort() == this || connection->endPort() == this)
            {
                toRemove.push_back(connection);
            }
        }

        // 删除所有相关连接
        for (QBlueprintConnection *connection : toRemove)
        {
            blueprintView->removeConnection(connection);
        }
    }
}

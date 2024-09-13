#include "qblueprintconnection.h"

QBlueprintConnection::QBlueprintConnection(QBlueprintPort *startPort, QBlueprintPort *endPort, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_startPort(startPort), m_endPort(endPort)
{
    // 初始化起点和终点坐标
    m_startPoint = startPort->centerPos();
    if (endPort)
        m_endPoint = endPort->centerPos();
    else
        m_endPoint = m_startPoint;  // 如果没有终点端口，临时设置终点为起点

    updatePosition(m_startPoint, m_endPoint);  // 初始化位置
}

void QBlueprintConnection::updatePosition(const QPointF &startPos, const QPointF &endPos)
{
    // 更新起点和终点
    prepareGeometryChange();  // 通知 Qt 几何形状发生变化
    m_startPoint = startPos;
    m_endPoint = endPos;
    update();  // 刷新图形项
}

QRectF QBlueprintConnection::boundingRect() const
{
    // 计算连接线的边界矩形
    return QRectF(m_startPoint, m_endPoint).normalized();
}

void QBlueprintConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 设置连接线的样式
    QPen pen(Qt::white, 2);  // 2 像素宽的白色线条
    painter->setPen(pen);

    // 绘制连接线
    painter->drawLine(m_startPoint, m_endPoint);
}

void QBlueprintConnection::setEndPort(QBlueprintPort *endPort)
{
    m_endPort = endPort;
    if (m_endPort)
    {
        // 如果存在终点端口，更新终点坐标为端口的中心
        updatePosition(m_startPort->centerPos(), m_endPort->centerPos());
    }
}

#include "qblueprintconnection.h"
#include <QPainterPath>

QBlueprintConnection::QBlueprintConnection(QBlueprintPort *startPort, QBlueprintPort *endPort, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_startPort(startPort), m_endPort(endPort)
{
    // 初始化起点和终点坐标
    m_startPoint = startPort->centerPos();
    if (endPort)
        m_endPoint = endPort->centerPos();
    else
        m_endPoint = m_startPoint;  // 如果没有终点端口，临时设置终点为起点
    setZValue(2);
    setFlag(QGraphicsItem::ItemIsSelectable, false);

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
    return QRectF(m_startPoint, m_endPoint).normalized().adjusted(-5, -5, 5, 5);
}

void QBlueprintConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 设置连接线的样式
    QPen pen(Qt::white, 2);  // 2 像素宽的白色线条
    painter->setPen(pen);

    // 创建贝塞尔曲线
    QPainterPath path(m_startPoint);

    // 计算控制点，控制点可以调整曲线的形状
    QPointF controlPoint1;
    QPointF controlPoint2;

    // 根据起点和终点的位置调整控制点
    qreal dx = m_endPoint.x() - m_startPoint.x();
    qreal dy = m_endPoint.y() - m_startPoint.y();
    qreal offset = qAbs(dx) * 0.5; // 控制点偏移量，值越大曲线弯曲越明显

    // 如果起点在终点的左侧
    if (dx > 0)
    {
        controlPoint1 = m_startPoint + QPointF(offset, 0);
        controlPoint2 = m_endPoint - QPointF(offset, 0);
    }
    else
    {
        // 如果起点在终点的右侧，曲线向外弯曲
        controlPoint1 = m_startPoint + QPointF(offset, 0);
        controlPoint2 = m_endPoint - QPointF(offset, 0);
        controlPoint1.setY(m_startPoint.y() - qAbs(dx) * 0.2); // 向上或向下调整控制点
        controlPoint2.setY(m_endPoint.y() + qAbs(dx) * 0.2);
    }

    // 创建二次贝塞尔曲线
    path.cubicTo(controlPoint1, controlPoint2, m_endPoint);

    // 绘制曲线
    painter->drawPath(path);
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
QBlueprintPort* QBlueprintConnection::startPort() const
{
    return m_startPort;
}

QBlueprintPort* QBlueprintConnection::endPort() const
{
    return m_endPort;
}
QPainterPath QBlueprintConnection::shape() const
{
    QPainterPath path(m_startPoint);

    // 计算控制点
    QPointF controlPoint1 = m_startPoint + QPointF((m_endPoint.x() - m_startPoint.x()) / 2, 0);
    QPointF controlPoint2 = m_endPoint + QPointF((m_startPoint.x() - m_endPoint.x()) / 2, 0);

    path.cubicTo(controlPoint1, controlPoint2, m_endPoint);

    // 使用路径描边器生成较细的形状
    QPainterPathStroker stroker;
    stroker.setWidth(6); // 线的宽度
    return stroker.createStroke(path);
}


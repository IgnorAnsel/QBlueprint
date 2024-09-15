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
    QPen pen;

    if (isSelected)
    {
        // 使用渐变效果
        QLinearGradient gradient(m_startPoint, m_endPoint);
        gradient.setColorAt(0, Qt::white);
        gradient.setColorAt(0.5, Qt::yellow); // 在中间添加一个渐变点
        gradient.setColorAt(1, Qt::transparent);

        // 设置连接线的样式，将渐变应用到笔刷
        pen = QPen(QBrush(gradient), 2);  // 使用渐变作为画笔
    }
    else
    {
        // 使用普通线条
        pen = QPen(Qt::white, 2);
    }

    painter->setPen(pen);

    // 创建贝塞尔曲线
    QPainterPath path(m_startPoint);

    // 计算控制点
    QPointF controlPoint1;
    QPointF controlPoint2;

    // 获取起点和终点的位置差
    qreal dx = m_endPoint.x() - m_startPoint.x();
    qreal dy = m_endPoint.y() - m_startPoint.y();
    qreal offset = qAbs(dx) * 0.6; // 控制点偏移量

    if (m_startPort->portType() == QBlueprintPort::Output)
    {
        // 起点是 Output 端口
        if (dx > 0) // 终点在起点的右侧
        {
            controlPoint1 = m_startPoint + QPointF(offset, 0);
            controlPoint2 = m_endPoint - QPointF(offset, 0);
        }
        else // 终点在起点的左侧
        {
            controlPoint1 = m_startPoint + QPointF(offset, dy * 0.5); // 向下或向上调整控制点
            controlPoint2 = m_endPoint - QPointF(offset, -dy * 0.5);
        }
    }
    else if (m_startPort->portType() == QBlueprintPort::Input)
    {
        // 起点是 Input 端口
        if (dx > 0) // 终点在起点的右侧
        {
            controlPoint1 = m_startPoint - QPointF(offset, 0);
            controlPoint2 = m_endPoint + QPointF(offset, 0);
        }
        else // 终点在起点的左侧
        {
            controlPoint1 = m_startPoint - QPointF(offset, -dy * 0.5); // 向上或向下调整控制点
            controlPoint2 = m_endPoint + QPointF(offset, dy * 0.5);
        }
    }

    // 创建贝塞尔曲线
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

void QBlueprintConnection::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // 切换选中状态
    isSelected = !isSelected;

    // 重绘线条
    update();

    // 保持默认的事件处理
    QGraphicsItem::mousePressEvent(event);
}



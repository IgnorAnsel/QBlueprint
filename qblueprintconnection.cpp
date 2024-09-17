#include "qblueprintconnection.h"
#include <QPainterPath>
#include <QTimer>
#include "qblueprint.h"
QBlueprintConnection::QBlueprintConnection(QBlueprintPort *startPort, QBlueprintPort *endPort, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_startPort(startPort), m_endPort(endPort), animationProgress(0.0)
{
    // 初始化起点和终点坐标
    m_startPoint = startPort->centerPos();
    m_startColor = getColorFromType(startPort->getNodeType());
    if (endPort)
    {
        m_endPoint = endPort->centerPos();
        m_endColor = getColorFromType(endPort->getNodeType());
    }
    else
    {
        m_endPoint = m_startPoint;  // 如果没有终点端口，临时设置终点为起点
        m_endColor = QColor(Qt::yellow);
    }
    setZValue(2);
    setFlag(QGraphicsItem::ItemIsSelectable, false);

    updatePosition(m_startPoint, m_endPoint);  // 初始化位置

    // 初始化定时器
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        animationProgress += 0.01;  // 每次更新增加动画进度
        if (animationProgress > 1.0)  // 超过1则重置为0
            animationProgress = 0.0;
        update();  // 触发重绘
    });
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

        // 动态调整渐变的颜色位置, 使用多个渐变点模拟流动
        int numPoints = 20;  // 渐变点的数量
        for (int i = 0; i <= numPoints; ++i) {
            // 计算每个渐变点的位置和颜色
            qreal position = fmod(animationProgress + static_cast<double>(i) / numPoints, 1.0);
            QColor color = (i % 2 == 0) ? Qt::green : Qt::white;  // 交替颜色
            gradient.setColorAt(position, color);
        }

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
        if (dx > 0)
        {
            controlPoint1 = m_startPoint + QPointF(offset, 0);
            controlPoint2 = m_endPoint - QPointF(offset, 0);
        }
        else
        {
            controlPoint1 = m_startPoint + QPointF(offset, dy * 0.5);
            controlPoint2 = m_endPoint - QPointF(offset, -dy * 0.5);
        }
    }
    else if (m_startPort->portType() == QBlueprintPort::Input)
    {
        if (dx > 0)
        {
            controlPoint1 = m_startPoint - QPointF(offset, 0);
            controlPoint2 = m_endPoint + QPointF(offset, 0);
        }
        else
        {
            controlPoint1 = m_startPoint - QPointF(offset, -dy * 0.5);
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
    clearSelection();

    // 切换选中状态
    isSelected = true;
    animationTimer->start(50);  // 启动动画，每50毫秒更新一次

    // 重绘线条
    update();

    // 保持默认的事件处理
    QGraphicsItem::mousePressEvent(event);
}
// 静态方法：清除所有连接的选中状态
void QBlueprintConnection::clearSelection()
{
    QGraphicsScene *currentScene = this->scene();
    if (!currentScene) return;

    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());
    if (blueprintView)
    {
        for (QBlueprintConnection *connection : blueprintView->connections)
        {
            connection->isSelected = false;
            connection->animationTimer->stop();  // 停止动画
            connection->update();
        }
    }
}

QColor QBlueprintConnection::getColorFromType(enum Type type)
{
    if (type == Type::FUNCTION)
        return(QColor(0, 128, 255));  // 颜色为蓝色
    else if (type == Type::INPUT)
        return(QColor(0, 255, 0));  // 颜色为绿色
    else if (type == Type::OUTPUT)
        return(QColor(255, 0, 0));  // 颜色为红色
}


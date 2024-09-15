#ifndef QBLUEPRINTCONNECTION_H
#define QBLUEPRINTCONNECTION_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>
#include "qblueprintport.h"
#include <QPainterPathStroker>
#include <QTimer>
#include <QObject>
class QBlueprintConnection : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    QBlueprintConnection(QBlueprintPort *startPort, QBlueprintPort *endPort, QGraphicsItem *parent = nullptr);

    // 更新连接的起点和终点位置
    void updatePosition(const QPointF &startPos, const QPointF &endPos);

    QRectF boundingRect() const override;  // 必须实现，用于定义图形项的边界矩形
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;  // 必须实现，用于绘制项

    // 设置终点端口
    void setEndPort(QBlueprintPort *endPort);

    QBlueprintPort *endPort() const;
    QBlueprintPort *startPort() const;
protected:
    QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QBlueprintPort *m_startPort;  // 起始端口
    QBlueprintPort *m_endPort;    // 终点端口
    QPointF m_startPoint;         // 连线的起点坐标
    QPointF m_endPoint;           // 连线的终点坐标
    bool isSelected = false;      // 标记线条是否被选中
    QTimer* animationTimer;  // 定时器用于驱动动画
    qreal animationProgress;  // 动画进度 [0, 1]
    void setupAnimation();  // 设置动画
    void clearSelection();
};

#endif // QBLUEPRINTCONNECTION_H

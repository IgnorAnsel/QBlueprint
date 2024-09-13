#ifndef QBLUEPRINTNODE_H
#define QBLUEPRINTNODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <vector>
#include "qblueprintport.h"
#
class QBlueprintNode : public QGraphicsItem
{
public:
    QBlueprintNode(QGraphicsItem *parent = nullptr);

    // 重载boundingRect()方法，定义绘制区域
    QRectF boundingRect() const override;

    // 重载paint()方法，定义绘制节点的具体方式
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 添加输入和输出端口
    void addInputPort();
    void addOutputPort();
    // 添加输入和输出端口（带名称）
    void addInputPort(const QString &name);
    void addOutputPort(const QString &name);

protected:
    // 鼠标交互事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    std::vector<QBlueprintPort *> inputPorts;   // 存储输入端口
    std::vector<QBlueprintPort *> outputPorts;  // 存储输出端口
};

#endif // QBLUEPRINTNODE_H

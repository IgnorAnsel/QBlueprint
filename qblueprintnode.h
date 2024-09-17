#ifndef QBLUEPRINTNODE_H
#define QBLUEPRINTNODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <vector>
#include "alluse.h"
#include "qblueprintport.h"
#include "qblueprintconnection.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
class QBlueprintNode : public QGraphicsItem
{
public:

    QBlueprintNode(QGraphicsItem *parent = nullptr);
    ~QBlueprintNode();
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

    const std::vector<QBlueprintPort *>& getInputPorts() const { return inputPorts; }
    const std::vector<QBlueprintPort *>& getOutputPorts() const { return outputPorts; }

    void setNodeTitle(QString name);
    void setClassName(QString class_name);
    // 获取类名
    QString getClassName() const { return class_name; }
    // 获取节点名称
    QString getNodeTitle() const { return m_name; }
    enum Type getNodeType() { return nodeType; }
    enum Type setNodeType(enum Type type){ nodeType = type; }
    QBlueprintNode *clone() const;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    enum Type nodeType;
    QString class_name;
    QString m_name;
    std::vector<QBlueprintPort *> inputPorts;   // 存储输入端口
    std::vector<QBlueprintPort *> outputPorts;  // 存储输出端口

};

#endif // QBLUEPRINTNODE_H

#ifndef QBLUEPRINTPORT_H
#define QBLUEPRINTPORT_H

#include <QGraphicsItem>
#include <QPainter>
class QBlueprintConnection;
class QBlueprint;
class QBlueprintPort : public QGraphicsItem
{
public:
    enum PortType { Input, Output };  // 定义端口类型：输入和输出

    QBlueprintPort(PortType type, const QString &name, QGraphicsItem *parent);
    virtual QBlueprintPort* clone() const;

    // 设置端口的矩形区域
    QRectF boundingRect() const override;

    // 绘制端口
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 获取端口的中心点，用于连接线
    QPointF centerPos() const;

    PortType portType() const { return m_type; }

    // 设置和获取端口名称
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    void updateConnections();
    void removeConnections();
protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    std::vector<QBlueprintConnection*> connections;  // 存储连接的指针
    PortType m_type;
    QString m_name;  // 端口名称
};

#endif // QBLUEPRINTPORT_H

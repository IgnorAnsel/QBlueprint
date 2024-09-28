#ifndef QBLUEPRINTPORT_H
#define QBLUEPRINTPORT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include "alluse.h"
class QBlueprintConnection;
class QBlueprintPort :  public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum PortType { Input, Output, EVENT_OUTPUT, EVENT_INPUT};  // 定义端口类型：输入和输出
    QFont m_font;
    void setPortFont(const QFont &font) {
        m_font = font;
    }
    QBlueprintPort(PortType type, const QString &name, DataType dataType, QGraphicsItem *parent,QString brief);
    virtual QBlueprintPort* clone() const;

    // 设置端口的矩形区域
    QRectF boundingRect() const override;

    // 绘制端口
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 获取端口的中心点，用于连接线
    QPointF centerPos() const;

    PortType portType() const { return m_type; }
    DataType portDataType() const { return dataType; }
    // 设置和获取端口名称
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    enum Type getNodeType() { return parentnodeType; }
    enum Type setNodeType(enum Type type){ parentnodeType = type; }
    void updateConnections();
    void removeConnections();
    void sendDataToConnectedPorts();
    void receiveData(const QVariant &data);
    void setVarType(const QVariant &value);
    void setPortBrief(QString portBrief);
    void initPortBrief();
    QString getVarTypeName() const; // 获取 var 类型名称的方法
    void setData(const QVariant& data) {
        var = data;
        emit dataUpdated(var);
    }

    QVariant data() const {
        return var;
    }
signals:
    void dataUpdated(const QVariant &data);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QString portBrief;
    DataType dataType;
    QVariant var;
    enum Type parentnodeType;
    std::vector<QBlueprintConnection*> connections;  // 存储连接的指针
    PortType m_type;
    QString m_name;  // 端口名称
    void setQVariantType();
};

#endif // QBLUEPRINTPORT_H

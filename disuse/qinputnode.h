#ifndef QINPUTNODE_H
#define QINPUTNODE_H

#include "qblueprintnode.h"
#include <QLabel>
#include <QGraphicsProxyWidget>

class QInputNode : public QBlueprintNode // 输入节点,用于接收输入,显示数据
{
public:
    QInputNode(QGraphicsItem* parent = nullptr)
        : QBlueprintNode(parent) {
        setNodeTitle("Input Node");
        addInputPort("Input");
        setNodeType(Type::INPUT);
        qDebug() << "input" << nodeType;
        // 创建 QLabel 作为输出显示的 widget
        outputLabel = new QLabel("0");
        outputLabel->setFixedSize(80, 20);

        // 将 QLabel 添加到 QGraphicsItem
        proxyWidget = new QGraphicsProxyWidget(this);
        proxyWidget->setWidget(outputLabel);
        proxyWidget->setPos(boundingRect().width() / 2 - outputLabel->width() / 2, 50);
    }

    // 设置输出值
    void setOutputValue(int value) {
        outputLabel->setText(QString::number(value));
    }

protected:
    //QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        QBlueprintNode::paint(painter, option, widget);

        // 绘制输出字段区域（已经在 QLabel 中绘制）
        painter->setPen(Qt::black);
        painter->setBrush(Qt::black);
    }

private:
    enum Type nodeType;
    QLabel* outputLabel;  // 输出字段
    QGraphicsProxyWidget* proxyWidget;  // QLabel 的代理
};

#endif // QOUTPUTNODE_H


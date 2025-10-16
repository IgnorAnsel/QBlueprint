#ifndef QOUTPUTNODE_H
#define QOUTPUTNODE_H

#include "../qblueprintnode.h"
#include <QLineEdit>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsProxyWidget>
#include <QIntValidator>
class QOutputNode : public QBlueprintNode // 输出节点,用于自行输出数据
{
public:
    QOutputNode(QGraphicsItem* parent = nullptr)
        : QBlueprintNode(parent) {
        setNodeTitle("Output Node");
        addOutputPort("Output");
        setNodeType(Type::OUTPUT);
        // 创建 QLineEdit 作为用户输入的 widget
        inputField = new QLineEdit();
        inputField->setValidator(new QIntValidator(0, 1000));  // 只允许输入整数
        inputField->setFixedSize(80, 20);

        // 将 QLineEdit 添加到 QGraphicsItem
        proxyWidget = new QGraphicsProxyWidget(this);
        proxyWidget->setWidget(inputField);
        proxyWidget->setPos(boundingRect().width() / 2 - inputField->width() / 2, 50);
    }

    // 设置输出值
    int getValue() const {
        return inputField->text().toInt();
    }

protected:

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        QBlueprintNode::paint(painter, option, widget);

        // 绘制输出字段区域（已经在 QLabel 中绘制）
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
    }

private:
    QLineEdit* inputField;  // 输入字段
    QGraphicsProxyWidget* proxyWidget;  // QLineEdit 的代理
};

#endif // QOUTPUTNODE_H

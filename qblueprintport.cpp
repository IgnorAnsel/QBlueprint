#include "qblueprintport.h"

QBlueprintPort::QBlueprintPort(PortType type, const QString &name, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type), m_name(name)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

}

QRectF QBlueprintPort::boundingRect() const
{
    //return QRectF(-10, -10, 20, 20);  // 扩大范围以便检测
    return QRectF(0, 0, 10, 10);  // 端口的大小为 10x10
    //qDebug() << "QBlueprintPort boundingRect:" << rect;

}

void QBlueprintPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 绘制端口（不同类型的端口使用不同的颜色）
    painter->setBrush((m_type == Input) ? Qt::blue : Qt::green);  // 输入端口为蓝色，输出端口为绿色
    painter->drawEllipse(boundingRect());  // 绘制圆形端口

    // 设置字体大小
    QFont font = painter->font();
    font.setPointSize(10);  // 调整字体大小
    painter->setFont(font);

    // 获取字体的度量信息，用来计算文本宽度
    QFontMetrics fontMetrics(font);

    // 动态计算端口名称的宽度
    int textWidth = fontMetrics.horizontalAdvance(m_name);
    int textHeight = fontMetrics.height();
    // 绘制端口名称，放在端口旁边
    painter->setPen(Qt::white);

    if (m_type == Input)
    {
        // 输入端口：名称在端口的右边，左对齐
        QRectF textRect = boundingRect().translated(boundingRect().width() + 10, 0);  // 向右移动文本区域，留出10像素的间距
        textRect.setWidth(textWidth);  // 设置文本区域的宽度，确保名称的最后字符靠近端口
        textRect.setHeight(textHeight);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_name);  // 名称放在右侧，左对齐
    }
    else
    {
        // 输出端口：名称在端口的左边，右对齐
        QRectF textRect = boundingRect().translated(-textWidth - 10, 0);  // 根据文本长度动态调整区域，留出10像素的间距
        textRect.setWidth(textWidth);  // 设置文本区域的宽度，确保名称的最后字符靠近端口
        textRect.setHeight(textHeight);
        painter->drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, m_name);  // 名称放在左侧，右对齐
    }
}

QPointF QBlueprintPort::centerPos() const
{
    return mapToScene(boundingRect().center());  // 返回端口的中心点
}

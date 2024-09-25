#include "qblueprintport.h"
#include "qblueprintconnection.h"
#include "qblueprint.h"
QBlueprintPort::QBlueprintPort(PortType type, const QString &name, DataType dataType, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_type(type), m_name(name),m_font(QFont("Arial", 10)),dataType(dataType)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setQVariantType();
    setZValue(2);
}
QBlueprintPort* QBlueprintPort::clone() const
{
    // 创建一个新的 QBlueprintPort 实例并复制所需的属性
    QBlueprintPort* newPort = new QBlueprintPort(this->m_type, this->m_name, this->dataType, nullptr); // 注意这里 parent 设为 nullptr
    newPort->setNodeType(this->parentnodeType);
    // 复制属性
    return newPort;
}
QRectF QBlueprintPort::boundingRect() const
{
    //return QRectF(-10, -10, 20, 20);  // 扩大范围以便检测
    return QRectF(0, 0, 10, 10);  // 端口的大小为 10x10
    //qDebug() << "QBlueprintPort boundingRect:" << rect;
}

void QBlueprintPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setFont(m_font);  // 使用成员变量中的字体

    // 获取字体的度量信息，用来计算文本宽度
    QFontMetrics fontMetrics(m_font);
    int textWidth = fontMetrics.horizontalAdvance(m_name);
    int textHeight = fontMetrics.height();

    // 绘制端口
    if (m_type == EVENT_INPUT || m_type == EVENT_OUTPUT)
    {
        // 绘制朝右的三角形
        QPolygonF triangle;
        qreal size = 5; // 三角形的大小
        QPointF center = boundingRect().center(); // 获取中心位置

        // 定义三角形的三个顶点
        triangle << QPointF(center.x() - size, center.y() - size); // 左上顶点
        triangle << QPointF(center.x() - size, center.y() + size); // 左下顶点
        triangle << QPointF(center.x() + size, center.y());        // 右侧顶点

        painter->setBrush(Qt::white); // 设置三角形为白色
        painter->drawPolygon(triangle); // 绘制三角形
    }
    else
    {
        // 绘制圆形端口
        painter->setBrush((m_type == Input) ? Qt::blue : Qt::green);  // 输入端口为蓝色，输出端口为绿色
        painter->drawEllipse(boundingRect());  // 绘制圆形端口
    }

    // 绘制端口名称，放在端口旁边
    painter->setPen(Qt::white);
    if (m_type == Input || m_type == EVENT_INPUT)
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

void QBlueprintPort::updateConnections()
{
    // 检查当前场景是否存在
    QGraphicsScene *currentScene = this->scene();
    if (!currentScene) return;  // 如果场景不存在，直接返回

    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView)
    {
        // 更新该端口相关的所有连接
        blueprintView->updateConnectionsForPort(this);
    }
}

// 移除并删除该端口相关的所有连接
void QBlueprintPort::removeConnections()
{
    QGraphicsScene *currentScene = this->scene();
    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView)
    {
        std::vector<QBlueprintConnection*> toRemove;
        // 遍历所有连接，收集与此端口相关的连接
        for (QBlueprintConnection *connection : blueprintView->connections)
        {
            if (connection->startPort() == this || connection->endPort() == this)
            {
                toRemove.push_back(connection);
            }
        }

        // 删除所有相关连接
        for (QBlueprintConnection *connection : toRemove)
        {
            blueprintView->removeConnection(connection);
        }
    }
}

void QBlueprintPort::sendDataToConnectedPorts() {
    // 获取当前场景
    QGraphicsScene *currentScene = this->scene();
    if (!currentScene) return;

    // 获取 QBlueprint 视图
    QBlueprint *blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView) {
        // 遍历所有连接，找到与当前端口相连的端口
        for (QBlueprintConnection *connection : blueprintView->connections) {
            if (connection->startPort() == this) {
                QBlueprintPort* targetPort = connection->endPort();
                if (targetPort) {
                    QVariant convertedVar;
                    switch (targetPort->portDataType()) {
                    case DataType::INT:
                        convertedVar = QVariant::fromValue(var.toInt());  // 转换为 int 类型
                        break;
                    case DataType::FLOAT:
                        convertedVar = QVariant::fromValue(var.toFloat());  // 转换为 float 类型
                        break;
                    case DataType::DOUBLE:
                        convertedVar = QVariant::fromValue(var.toDouble());  // 转换为 double 类型
                        break;
                    case DataType::STRING:
                        convertedVar = QVariant::fromValue(var.toString());  // 转换为 QString 类型
                        break;
                    case DataType::BOOL:
                        convertedVar = QVariant::fromValue(var.toBool());  // 转换为 bool 类型
                        break;
                    case DataType::LONG:
                        convertedVar = QVariant::fromValue(var.toLongLong());  // 转换为 long 类型
                        break;
                    case DataType::SHORT:
                        convertedVar = QVariant::fromValue(static_cast<short>(var.toInt()));  // 转换为 short 类型
                        break;
                    case DataType::UNSIGNED_INT:
                        convertedVar = QVariant::fromValue(var.toUInt());  // 转换为 unsigned int 类型
                        break;
                    case DataType::QIMAGE:
                        if (var.canConvert<QImage>()) {
                            QImage image = var.value<QImage>();  // 获取 QImage 对象
                            convertedVar = QVariant::fromValue(image);  // 将 QImage 转换为 QVariant 并存储
                        }
                        break;
                    // 你可以根据需求添加其他类型转换逻辑...
                    default:
                        convertedVar = var;  // 如果未匹配到类型，保持原样
                        break;
                    }
                    // 发送转换后的数据给 targetPort
                    qDebug() << "Sending converted data from" << this->name() << "to" << targetPort->name();
                    targetPort->receiveData(convertedVar);
                    // 发送数据给 targetPort
                    // qDebug() << "Sending data from" << this->name() << "to" << targetPort->name();
                }
            }
        }
    }
}


// void QBlueprintPort::receiveData(const QVariant &data) {
//     // 更新端口的变量数据
//     var = data;
//     qDebug() << "有数据";
//     // 通知父节点更新 UI 或其他逻辑
//     QBlueprintNode* parentNode = dynamic_cast<QBlueprintNode*>(parentItem());
//     if (parentNode) {
//         parentNode->updateLabelWithData(this, data.toString());
//     }
//     sendDataToConnectedPorts();
// }
void QBlueprintPort::receiveData(const QVariant &data) {
    if (data.canConvert<QImage>()) {
        QImage image = data.value<QImage>();
        if (!image.isNull()) {
            qDebug() << "Received valid QImage data of size:" << image.size();
        } else {
            qWarning() << "Received QImage but it is null.";
        }
    } else {
        qWarning() << "Invalid data received: " << data;
    }
    setData(data);
    // 通知父节点处理数据
    QBlueprintNode* parentNode = dynamic_cast<QBlueprintNode*>(parentItem());
    if (parentNode) {
        parentNode->processData(this, data);
    }
}





void QBlueprintPort::setVarType(const QVariant &value)
{
    var = value;
}

QString QBlueprintPort::getVarTypeName() const
{
    return var.typeName();
}
void QBlueprintPort::setQVariantType()
{
    switch (dataType) {
    case DataType::INT:
        setVarType(QVariant::fromValue(int())); // 设置为 int 类型
        break;
    case DataType::FLOAT:
        setVarType(QVariant::fromValue(float())); // 设置为 float 类型
        break;
    case DataType::DOUBLE:
        setVarType(QVariant::fromValue(double())); // 设置为 double 类型
        break;
    case DataType::CHAR:
        setVarType(QVariant::fromValue(char())); // 设置为 char 类型
        break;
    case DataType::STRING:
        setVarType(QVariant::fromValue(QString())); // 设置为 QString 类型
        break;
    case DataType::BOOL:
        setVarType(QVariant::fromValue(bool())); // 设置为 bool 类型
        break;
    case DataType::LONG:
        setVarType(QVariant::fromValue(qint64())); // 设置为 long 类型 (qint64)
        break;
    case DataType::SHORT:
        setVarType(QVariant::fromValue(short())); // 设置为 short 类型
        break;
    case DataType::UNSIGNED_INT:
        setVarType(QVariant::fromValue(uint())); // 设置为 unsigned int 类型
        break;
    case DataType::VARIANT:
        setVarType(QVariant()); // QVariant 自身
        break;
    case DataType::QSTRING:
        setVarType(QVariant::fromValue(QString())); // 设置为 QString 类型
        break;
    case DataType::QTIME:
        setVarType(QVariant::fromValue(QTime())); // 设置为 QTime 类型
        break;
    case DataType::QPOINT:
        setVarType(QVariant::fromValue(QPoint())); // 设置为 QPoint 类型
        break;
    case DataType::QPOINTF:
        setVarType(QVariant::fromValue(QPointF())); // 设置为 QPointF 类型
        break;
    case DataType::QSIZE:
        setVarType(QVariant::fromValue(QSize())); // 设置为 QSize 类型
        break;
    case DataType::QSIZEF:
        setVarType(QVariant::fromValue(QSizeF())); // 设置为 QSizeF 类型
        break;
    case DataType::QRECT:
        setVarType(QVariant::fromValue(QRect())); // 设置为 QRect 类型
        break;
    case DataType::QRECTF:
        setVarType(QVariant::fromValue(QRectF())); // 设置为 QRectF 类型
        break;
    case DataType::QCOLOR:
        setVarType(QVariant::fromValue(QColor())); // 设置为 QColor 类型
        break;
    case DataType::QPIXMAP:
        setVarType(QVariant::fromValue(QPixmap())); // 设置为 QPixmap 类型
        break;
    case DataType::QIMAGE:
        setVarType(QVariant::fromValue(QImage())); // 设置为 QImage 类型
        break;
    default:
        setVarType(QVariant()); // 默认设置为一个空的 QVariant
        break;
    }
}

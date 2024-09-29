#include "qblueprintnode.h"
#include "enterlimiter.h"
#include <QDebug>
#include "qblueprint.h"
QBlueprintNode::QBlueprintNode(enum Type Type, DataType datatype, QGraphicsItem *parent)
    : QGraphicsItem(parent),dataType(datatype)
{
    // 启用拖动（节点可以被鼠标拖动）
    setFlag(QGraphicsItem::ItemIsMovable);
    // 启用选择（节点可以被点击选中）
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);
    setZValue(1);
    dataType = datatype;
    setNodeType(Type);
    if(nodeType == Type::FUNCTION || nodeType == Type::BRANCH || nodeType == Type::CONDITION || nodeType == Type::FORLOOP)
        customNodePortSort();
    else
        addButtonToTopLeft();
}
QBlueprintNode::~QBlueprintNode()
{
    // 删除节点时，移除所有连接
    for (QBlueprintPort *port : inputPorts)
    {
        port->removeConnections();
    }
    for (QBlueprintPort *port : outputPorts)
    {
        port->removeConnections();
    }
}
QBlueprintNode* QBlueprintNode::clone() const
{
    // 创建一个新的 QBlueprintNode 实例
    QBlueprintNode* newNode = new QBlueprintNode(this->nodeType,this->dataType);
    newNode->setNodeTitle(this->m_name);
    newNode->setClassName(this->class_name);
    newNode->setNodeType(this->nodeType);
    qDebug() << "node type :" << nodeType;
    if(newNode->nodeType != Type::INPUT && newNode->nodeType != Type::BRANCH && newNode->nodeType != Type::CONDITION && newNode->nodeType != Type::FORLOOP) // 输入节点和控制节点是不需要添加事件端口的
    {
        newNode->addInputPort(Type::INPUT); // 添加事件端口
        newNode->addOutputPort(Type::OUTPUT);
    }
    else if(newNode->nodeType == Type::BRANCH)
    {
        newNode->addInputPort(Type::BRANCH);
        newNode->addOutputPort(Type::BRANCH);
    }
    else if(newNode->nodeType == Type::CONDITION)
    {
        newNode->addInputPort(Type::CONDITION);
        newNode->addOutputPort(Type::CONDITION);
    }
    else if(newNode->nodeType == Type::FORLOOP)
    {
        newNode->addInputPort(Type::FORLOOP);
        newNode->addOutputPort(Type::FORLOOP);
    }
    // 克隆输入端口
    for (QBlueprintPort* port : this->inputPorts) {
        QBlueprintPort* clonedPort = port->clone();
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->inputPorts.push_back(clonedPort);
    }

    // 克隆输出端口
    for (size_t i = 0; i < this->outputPorts.size(); ++i) {
        QBlueprintPort* port = this->outputPorts[i];
        QBlueprintPort* clonedPort = port->clone(); // 假设 QBlueprintPort 有一个 clone 方法
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->outputPorts.push_back(clonedPort);
    }
    // 设置克隆节点的初始位置
    newNode->setPos(this->pos());

    return newNode;
}



QRectF QBlueprintNode::boundingRect() const
{
    QFont font;
    QFontMetrics fontMetrics(font);

    // 计算输入端口名称的最大宽度
    int maxInputWidth = 0;
    for (const auto& port : inputPorts)
    {
        int textWidth = fontMetrics.horizontalAdvance(port->name());
        if (textWidth > maxInputWidth)
        {
            maxInputWidth = textWidth;
        }
    }

    // 计算输出端口名称的最大宽度
    int maxOutputWidth = 0;
    for (const auto& port : outputPorts)
    {
        int textWidth = fontMetrics.horizontalAdvance(port->name());
        if (textWidth > maxOutputWidth)
        {
            maxOutputWidth = textWidth;
        }
    }

    int padding = 80;
    int nodeWidth = maxInputWidth + maxOutputWidth + padding;
    int nodeHeight = std::max(inputPorts.size(), outputPorts.size()) * 31 + 31;

    if (nodeType == Type::INPUT)
    {
        switch (dataType) {
        case DataType::INT:
        case DataType::FLOAT:
        case DataType::DOUBLE:
        case DataType::CHAR:
        case DataType::STRING:
        case DataType::BOOL:
        case DataType::LONG:
        case DataType::SHORT:
        case DataType::UNSIGNED_INT:
        case DataType::QSTRING:
        {
            int maxLineEditWidth = 0;
            for(const auto& lineEdit : lineEdits)
            {
                if(maxLineEditWidth < lineEdit->width())
                    maxLineEditWidth = lineEdit->width();
            }
            nodeWidth += maxLineEditWidth;
        }
            break;
        case DataType::QIMAGE:
            nodeWidth += 110;
            nodeHeight += outputPorts.size() * 90;
            break;
        default:
            break;
        }
    }
    else if(nodeType == Type::OUTPUT)
    {
        switch (dataType) {
        case DataType::INT:
        case DataType::FLOAT:
        case DataType::DOUBLE:
        case DataType::CHAR:
        case DataType::STRING:
        case DataType::BOOL:
        case DataType::LONG:
        case DataType::SHORT:
        case DataType::UNSIGNED_INT:
        case DataType::QSTRING:
        {
            int maxLabelWidth = 0;
            for(const auto& label : outputlabel)
            {
                if(maxLabelWidth < label->width())
                    maxLabelWidth = label->width();
            }
            nodeWidth += maxLabelWidth;
            break;
        }
        case DataType::QIMAGE:
        {
            int maxLabelWidth = 150;
            nodeWidth += maxLabelWidth;
            nodeHeight += (outputPorts.size()-1) * 65;
            break;
        }
        default:
            break;
        }
    }
    return QRectF(0, 0, nodeWidth, nodeHeight);
}


void QBlueprintNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 设置节点的背景为圆角矩形
    QRectF rect = boundingRect();
    painter->setBrush(Qt::gray);
    painter->setPen(Qt::black);
    painter->drawRoundedRect(rect, 10, 10);  // 绘制圆角矩形，10像素圆角

    // 绘制内边框
    QRectF innerRect = rect.adjusted(2, 2, -2, -2);  // 调整rect以绘制一个稍小的矩形，形成内边框效果
    painter->setBrush(Qt::NoBrush);  // 内边框不填充
    // 根据 nodeType 设置不同的内边框颜色
    if (nodeType == Type::FUNCTION)
        painter->setPen(QPen(QColor(0, 128, 255), 2));  // 设置内边框颜色为蓝色，宽度为2像素
    else if (nodeType == Type::INPUT)
        painter->setPen(QPen(QColor(0, 255, 0), 2));  // 设置内边框颜色为绿色，宽度为2像素
    else if (nodeType == Type::OUTPUT)
        painter->setPen(QPen(QColor(255, 0, 0), 2));  // 设置内边框颜色为红色，宽度为2像素
    painter->drawRoundedRect(innerRect, 8, 8);  // 绘制内边框，圆角稍微小一点

    // 设置字体大小并绘制标题
    QFont font = painter->font();
    font.setPointSize(10);  // 设置字体大小
    painter->setFont(font);
    painter->setPen(Qt::black);

    // 设置标题区域
    QRectF titleRect = QRectF(rect.left(), rect.top(), rect.width(), 30);  // 标题区域高度为30
    painter->drawText(titleRect, Qt::AlignCenter, m_name);  // 居中绘制标题

    // 在标题和端口区域之间绘制一条分割线
    painter->setPen(QPen(Qt::black, 1));  // 设置线条颜色和宽度
    painter->drawLine(rect.left(), titleRect.bottom(), rect.right(), titleRect.bottom());
    // 绘制端口区域，端口从下方开始排列
    if(dataType == DataType::QIMAGE)
        imageNodePortSort();
    else
        customNodePortSort();
}
void QBlueprintNode::addButtonToTopLeft()
{
    // 创建 QPushButton
    QPushButton* button = new QPushButton("+");
    // 创建 QGraphicsProxyWidget 并将按钮嵌入其中
    QGraphicsProxyWidget* buttonProxy = new QGraphicsProxyWidget(this);
    // 设置按钮的大小，按钮应该是一个正方形以便显示成圆形
    int buttonSize = 10;
    button->setFixedSize(buttonSize, buttonSize);
    // 使用样式表将按钮绘制成一个圆
    button->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"  // 按钮背景颜色（绿色）
        "    color: white;"  // 按钮文字颜色（白色）
        "    border: none;"  // 无边框
        "    border-radius: 0px;"  // 设置圆角半径，半径为按钮尺寸的一半
        "}"
        "QPushButton:pressed {"
        "    background-color: #45a049;"  // 点击时的背景颜色
        "}"
        );
    buttonProxy->setWidget(button);
    buttonProxy->setPos(10, 10);

    // 设置按钮的点击事件
    connect(button, &QPushButton::clicked, [this]() {
        if(nodeType == Type::INPUT)
        {
            switch (dataType) {
            case DataType::INT:
            case DataType::FLOAT:
            case DataType::DOUBLE:
            case DataType::CHAR:
            case DataType::STRING:
            case DataType::BOOL:
            case DataType::LONG:
            case DataType::SHORT:
            case DataType::UNSIGNED_INT:
            case DataType::QSTRING:{
                addLineEdit(addOutputPort(getEnumName(dataType)));
                break;
            }
            case DataType::QIMAGE:{
                QBlueprintPort * port = addOutputPort(getEnumName(dataType));
                imageNodePortSort();
                addInputLabel(port);
                break;
            }
            default:
                break;
            }

        }
        else if(nodeType == Type::OUTPUT){
            switch (dataType) {
            case DataType::INT:
            case DataType::FLOAT:
            case DataType::DOUBLE:
            case DataType::CHAR:
            case DataType::STRING:
            case DataType::BOOL:
            case DataType::LONG:
            case DataType::SHORT:
            case DataType::UNSIGNED_INT:
            case DataType::QSTRING:
            {
                QBlueprintPort * outputport = addOutputPort(getEnumName(dataType));
                QBlueprintPort * inputport = addInputPort(getEnumName(dataType));
                customNodePortSort();
                addOutputLabel(outputport,inputport);
                break;
            }
            case DataType::QIMAGE:{
                QBlueprintPort * outputport = addOutputPort(getEnumName(dataType));
                QBlueprintPort * inputport = addInputPort(getEnumName(dataType));
                imageNodePortSort();
                addOutputLabel(outputport,inputport);
                break;
            }
            default:
                break;
            }

        }

        qDebug() << "Button clicked!";
    });
}


QBlueprintPort* QBlueprintNode::addInputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, "NULL", dataType, this, getEnumName(dataType));
    port->setNodeType(nodeType);
    setQVariantType(port);
    inputPorts.push_back(port);
    return port;
}

QBlueprintPort* QBlueprintNode::addOutputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "NULL", dataType, this, getEnumName(dataType));
    port->setNodeType(nodeType);
    setQVariantType(port);
    outputPorts.push_back(port);
    return port;
}

QBlueprintPort* QBlueprintNode::addInputPort(const QString &name)
{
    DataType datatype = dataType;
    if(dataType == DataType::FOR_FUNCTION)
    {
        datatype = getEnumFromName(name);
    }
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, name, datatype, this, getEnumName(datatype));
    port->setNodeType(nodeType);
    setQVariantType(port);
    inputPorts.push_back(port);
    return port;
}

QBlueprintPort* QBlueprintNode::addOutputPort(const QString &name)
{
    DataType datatype = dataType;
    if(dataType == DataType::FOR_FUNCTION)
    {
        datatype = getEnumFromName(name);
        qDebug() << "eaesaeasease" << datatype;
    }
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, name, datatype, this, getEnumName(datatype));
    port->setNodeType(nodeType);
    setQVariantType(port);
    outputPorts.push_back(port);
    return port;
}
void QBlueprintNode::addInputPort(enum Type Type)
{
    if(Type == Type::FUNCTION)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::EVENT_INPUT, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        inputPorts.push_back(port);
    }
    else if(Type == Type::BRANCH)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::EVENT_INPUT, "", dataType, this, getEnumName(dataType));
        QBlueprintPort *port_conditon = new QBlueprintPort(QBlueprintPort::EVENT_CONDITION, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        port_conditon->setNodeType(nodeType);
        setQVariantType(port_conditon);
        inputPorts.push_back(port);
        inputPorts.push_back(port_conditon);
    }
    else if(Type == Type::CONDITION)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::EVENT_INPUT, "", dataType, this, getEnumName(dataType));
        QBlueprintPort *port2 = new QBlueprintPort(QBlueprintPort::EVENT_INPUT, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        port2->setNodeType(nodeType);
        setQVariantType(port2);
        inputPorts.push_back(port);
        inputPorts.push_back(port2);
    }
}
void QBlueprintNode::addOutputPort(enum Type Type)
{
    if(Type == Type::FUNCTION)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::EVENT_OUTPUT, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        outputPorts.push_back(port);
    }
    else if(Type == Type::BRANCH)
    {
        QBlueprintPort *port_return_true = new QBlueprintPort(QBlueprintPort::EVENT_TRUE_RETURN, "", dataType, this, getEnumName(dataType));
        QBlueprintPort *port_return_false = new QBlueprintPort(QBlueprintPort::EVENT_FALSE_RETURN, "", dataType, this, getEnumName(dataType));
        port_return_true->setNodeType(nodeType);
        setQVariantType(port_return_true);
        port_return_false->setNodeType(nodeType);
        setQVariantType(port_return_false);
        outputPorts.push_back(port_return_true);
        outputPorts.push_back(port_return_false);
    }
    else if(Type == Type::CONDITION)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        outputPorts.push_back(port);
    }
}

QBlueprintPort *QBlueprintNode::addInputPort(const QString &name, const QString &brief)
{
    DataType datatype = dataType;
    if(dataType == DataType::FOR_FUNCTION)
    {
        datatype = getEnumFromName(name);
    }
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, name, datatype, this, brief);
    port->setPortBrief(brief);
    port->setNodeType(nodeType);
    setQVariantType(port);
    inputPorts.push_back(port);
    return port;
}

QBlueprintPort *QBlueprintNode::addOutputPort(const QString &name, const QString &brief)
{
    DataType datatype = dataType;
    if(dataType == DataType::FOR_FUNCTION)
    {
        datatype = getEnumFromName(name);
    }
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, name, datatype, this, brief);
    port->setPortBrief(brief);
    port->setNodeType(nodeType);
    setQVariantType(port);
    outputPorts.push_back(port);
    return port;
}

void QBlueprintNode::setNodeTitle(QString name)
{
    m_name = name;
}

void QBlueprintNode::setClassName(QString class_name)
{
    QBlueprintNode::class_name = class_name;
}

QVariant QBlueprintNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        // 通知所有连接线进行更新
        for (QBlueprintPort *port : inputPorts)
        {
            port->updateConnections();
        }
        for (QBlueprintPort *port : outputPorts)
        {
            port->updateConnections();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void QBlueprintNode::setQVariantType(QBlueprintPort* port)
{
    switch (port->portDataType()) {
    case DataType::INT:
        port->setVarType(QVariant::fromValue(int())); // 设置为 int 类型
        break;
    case DataType::FLOAT:
        port->setVarType(QVariant::fromValue(float())); // 设置为 float 类型
        break;
    case DataType::DOUBLE:
        port->setVarType(QVariant::fromValue(double())); // 设置为 double 类型
        break;
    case DataType::CHAR:
        port->setVarType(QVariant::fromValue(char())); // 设置为 char 类型
        break;
    case DataType::STRING:
        port->setVarType(QVariant::fromValue(QString())); // 设置为 QString 类型
        break;
    case DataType::BOOL:
        port->setVarType(QVariant::fromValue(bool())); // 设置为 bool 类型
        break;
    case DataType::LONG:
        port->setVarType(QVariant::fromValue(qint64())); // 设置为 long 类型 (qint64)
        break;
    case DataType::SHORT:
        port->setVarType(QVariant::fromValue(short())); // 设置为 short 类型
        break;
    case DataType::UNSIGNED_INT:
        port->setVarType(QVariant::fromValue(uint())); // 设置为 unsigned int 类型
        break;
    case DataType::VARIANT:
        port->setVarType(QVariant()); // QVariant 自身
        break;
    case DataType::QSTRING:
        port->setVarType(QVariant::fromValue(QString())); // 设置为 QString 类型
        break;
    case DataType::QTIME:
        port->setVarType(QVariant::fromValue(QTime())); // 设置为 QTime 类型
        break;
    case DataType::QPOINT:
        port->setVarType(QVariant::fromValue(QPoint())); // 设置为 QPoint 类型
        break;
    case DataType::QPOINTF:
        port->setVarType(QVariant::fromValue(QPointF())); // 设置为 QPointF 类型
        break;
    case DataType::QSIZE:
        port->setVarType(QVariant::fromValue(QSize())); // 设置为 QSize 类型
        break;
    case DataType::QSIZEF:
        port->setVarType(QVariant::fromValue(QSizeF())); // 设置为 QSizeF 类型
        break;
    case DataType::QRECT:
        port->setVarType(QVariant::fromValue(QRect())); // 设置为 QRect 类型
        break;
    case DataType::QRECTF:
        port->setVarType(QVariant::fromValue(QRectF())); // 设置为 QRectF 类型
        break;
    case DataType::QCOLOR:
        port->setVarType(QVariant::fromValue(QColor())); // 设置为 QColor 类型
        break;
    case DataType::QPIXMAP:
        port->setVarType(QVariant::fromValue(QPixmap())); // 设置为 QPixmap 类型
        break;
    case DataType::QIMAGE:
        port->setVarType(QVariant::fromValue(QImage())); // 设置为 QImage 类型
        break;
    default:
        port->setVarType(QVariant()); // 默认设置为一个空的 QVariant
        break;
    }

}

void QBlueprintNode::customNodePortSort() {
    // 排列输入端口
    for (size_t i = 0; i < inputPorts.size(); ++i) {
        QFontMetrics fontMetrics(inputPorts[i]->m_font);
        int inputTextWidth = fontMetrics.horizontalAdvance(inputPorts[i]->name());
        inputPorts[i]->setPos(5, i * 30 + 40); // 左边距15，纵向位置
    }

    // 排列输出端口
    for (size_t i = 0; i < outputPorts.size(); ++i) {
        QFontMetrics fontMetrics(outputPorts[i]->m_font);
        int outputTextWidth = fontMetrics.horizontalAdvance(outputPorts[i]->name());
        outputPorts[i]->setPos(boundingRect().width() - 15, i * 30 + 40); // 右边距15
    }
}



void QBlueprintNode::imageNodePortSort() {
    if(nodeType == Type::INPUT)
    {
        for (size_t i = 0; i < inputPorts.size(); ++i) {
            // 将输入端口放在左侧
            inputPorts[i]->setPos(5, i * 115 + 35);  // 左边距
        }
        for (size_t i = 0; i < outputPorts.size(); ++i) {
            // 将输出端口放在右侧
            outputPorts[i]->setPos(boundingRect().width() - 15, i * 115 + 35); // 右边距
        }
    }
    else
    {
        int eventPortHeight = 30; // 事件端口所占高度
        int baseHeight = eventPortHeight; // 普通端口从这个高度开始排列
        // 首先，处理输入端口
        for (size_t i = 0; i < inputPorts.size(); ++i) {
            if (inputPorts[i]->portType() == QBlueprintPort::EVENT_INPUT) {
                // 事件端口固定在顶部位置
                inputPorts[i]->setPos(5, 40);  // 事件输入端口固定位置
            } else {
                // 普通输入端口从事件端口之后开始排列
                inputPorts[i]->setPos(5, baseHeight + (i-1) * 95 + 35);  // 左边距
            }
        }

        // 其次，处理输出端口
        for (size_t i = 0; i < outputPorts.size(); ++i) {
            if (outputPorts[i]->portType() == QBlueprintPort::EVENT_OUTPUT) {
                // 事件端口固定在顶部位置
                outputPorts[i]->setPos(boundingRect().width() - 15, 40);  // 事件输出端口固定位置
            } else {
                // 普通输出端口从事件端口之后开始排列
                outputPorts[i]->setPos(boundingRect().width() - 15, baseHeight + (i-1) * 95 + 35); // 右边距
            }
        }
    }

}

void QBlueprintNode::addLineEdit(QBlueprintPort* port)
{
    // 创建 QLineEdit
    QLineEdit* pLineEdit = new QLineEdit("");
    pLineEdit->setStyleSheet("QLineEdit { border: 1px solid black; border-radius: 0px; padding: 2px; }");

    // 创建 QGraphicsProxyWidget 并将 QLineEdit 添加到该代理
    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this); // 代理作为 QGraphicsItem 的子项
    pMyProxy->setWidget(pLineEdit); // 将 QWidget 基类对象添加到代理中

    // 设置较高的 Z 值，确保控件显示在前景
    pMyProxy->setZValue(10);
    lineEdits.push_back(pLineEdit);

    connect(pLineEdit, &QLineEdit::textChanged, this, &QBlueprintNode::adjustLineEditWidth);

    QPointF outputPortPos = port->pos();
    pMyProxy->setPos(outputPortPos.x() - pLineEdit->width() + 210, outputPortPos.y() + 35 + (outputPorts.size()-1) * 30);
    // connect(pLineEdit, &QLineEdit::textChanged, [port, this](const QString &text) {
    //     port->sendDataToConnectedPorts(text);  // 发送数据给所有连接的 input 端口
    // });
    // 设置克隆的 QLineEdit 大小与原始的一致
    pMyProxy->resize(QSize(60, 10));
    connect(pLineEdit, &QLineEdit::textChanged, [port, this](const QString &text) {
        // 将数据存储在 port 的变量中
        QVariant convertedValue;

        // 根据端口的数据类型转换输入的值
        switch (port->portDataType()) {
        case DataType::INT:
            convertedValue = QVariant::fromValue(text.toInt());
            break;
        case DataType::FLOAT:
            convertedValue = QVariant::fromValue(text.toFloat());
            break;
        case DataType::DOUBLE:
            convertedValue = QVariant::fromValue(text.toDouble());
            break;
        case DataType::BOOL:
            convertedValue = QVariant::fromValue(text.toLower() == "true" || text == "1");
            break;
        case DataType::QSTRING:
            convertedValue = QVariant::fromValue(text);
            break;
        case DataType::QPOINT:
            qDebug() << "neddthis ------";
            break;
        // 添加其他类型的处理逻辑
        default:
            convertedValue = QVariant::fromValue(text);  // 默认保存为字符串
            break;
        }

        // 将转换后的值存储到端口中
        port->setVarType(convertedValue);
        //port->setVarType(text);
        // 发送数据到连接的端口
        port->sendDataToConnectedPorts();
    });
    // 添加克隆的 QLineEdit 到新的节点的 lineEdits 列表
    setEnterLimiter(pLineEdit,port);
    lineEdits.push_back(pLineEdit);
}
void QBlueprintNode::adjustLineEditWidth(const QString &text) {
    QFontMetrics fontMetrics(font);

    // 计算文本宽度
    int textWidth = fontMetrics.horizontalAdvance(text);

    // 设置 QLineEdit 的宽度，添加一些额外的边距
    for (auto lineEdit : lineEdits) {
        lineEdit->setFixedWidth(textWidth + 20);  // 添加20像素的边距
    }
    // 通知场景准备重新计算节点的边界
    prepareGeometryChange();
    // 更新所有连接线的位置
    for (auto port : outputPorts) {
        port->updateConnections();
    }
    for (auto port : inputPorts) {
        port->updateConnections();
    }
}
void QBlueprintNode::adjustLabelWidth(const QString &text) {
    QFontMetrics fontMetrics(font);

    // 计算文本宽度
    int textWidth = fontMetrics.horizontalAdvance(text);

    // 设置 QLabel 的宽度，添加一些额外的边距
    for (auto label : outputlabel) {  // 假设 outputlabel 是存储所有 QLabel 的列表
        label->setFixedWidth(textWidth + 20);  // 添加20像素的边距
    }
}


void QBlueprintNode::addInputLabel(QBlueprintPort* port)
{
    // 创建 QWidget 作为 QGraphicsProxyWidget 的容器
    QWidget* containerWidget = new QWidget();
    containerWidget->setFixedSize(150, 110); // 设置容器大小，可以调整为合适的大小

    // 创建 QLabel 用于显示图片
    ImageLabel* pLabel = new ImageLabel(containerWidget); // 直接设置父对象为 containerWidget
    pLabel->setFixedSize(150, 90); // 设置 QLabel 大小
    pLabel->setStyleSheet("QLabel { border: 1px solid black; }");
    pLabel->setAlignment(Qt::AlignCenter); // 居中显示图片
    pLabel->move(0, 0); // 设置 QLabel 在容器中的位置

    // 创建 QPushButton 用于打开文件选择对话框
    QPushButton* pButton = new QPushButton("选择图片", containerWidget); // 直接设置父对象为 containerWidget
    pButton->setFixedSize(150, 20); // 设置按钮大小
    pButton->move(0, 90); // 设置 QPushButton 在容器中的位置

    // 创建 QGraphicsProxyWidget 并将 containerWidget 添加到代理
    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
    pMyProxy->setWidget(containerWidget);

    // 设置代理的位置
    QPointF inputPortPos = port->pos();
    qDebug() << inputPortPos;
    pMyProxy->setPos(inputPortPos.x() - 215, inputPortPos.y()); // 设置代理在场景中的位置
    inputlabel.push_back(pLabel);
    // 设置代理大小
    pMyProxy->resize(containerWidget->size());

    // 连接按钮点击事件，打开文件选择对话框
    connect(pButton, &QPushButton::clicked, [=]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "选择图片", "", "Images (*.png *.jpg *.bmp)");
        if (!filePath.isEmpty()) {
            QPixmap pixmap(filePath);
            pLabel->setPixmap(pixmap.scaled(pLabel->size(), Qt::KeepAspectRatio)); // 在 QLabel 中显示选中的图片
            QImage image(filePath);
            pLabel->setImage(image);
            // 更新输出端口的 QVariant 数据为 QImage
            port->setVarType(QVariant::fromValue(image));

            // 将数据发送给所有连接的端口
            port->sendDataToConnectedPorts();

            qDebug() << "Sending converted data from QImage to connected ports.";
        }
    });

    // 确保场景更新
    scene()->update();
}


void QBlueprintNode::addOutputLabel(QBlueprintPort *outport, QBlueprintPort *inport)
{
    QLabel* pLabel = nullptr; // 声明基类指针

    // 根据端口数据类型选择合适的 QLabel 类型
    if(outport->portDataType() == DataType::QIMAGE) {
        pLabel = new ImageLabel();  // 如果是 QIMAGE 类型，创建 ImageLabel 实例
    } else {
        pLabel = new QLabel("");
    }
    pLabel->setStyleSheet("QLineEdit { border: 1px solid black; border-radius: 0px; padding: 2px; }");

    // 创建 QGraphicsProxyWidget 并将 QLineEdit 添加到该代理
    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this); // 代理作为 QGraphicsItem 的子项
    pMyProxy->setWidget(pLabel); // 将 QWidget 基类对象添加到代理中

    // 设置较高的 Z 值，确保控件显示在前景
    pMyProxy->setZValue(10);
    pLabel->setMinimumWidth(100);
    pLabel->setMinimumHeight(10);
    QPointF outputPortPos = inport->pos();
    QFontMetrics fontMetrics(inport->m_font);
    int outputTextWidth = fontMetrics.horizontalAdvance(inport->name());
    pMyProxy->setPos(outputPortPos.x() + outputTextWidth + 30, outputPortPos.y() - 3);
    if (outport->portDataType() == DataType::QIMAGE){
        pMyProxy->resize(QSize(150, 90));
        connect(outport, &QBlueprintPort::dataUpdated, [pLabel](const QVariant &data) {
            if (data.canConvert<QImage>()) {
                QImage image = data.value<QImage>();
                if (!image.isNull()) {
                    // 转换 QImage 为 QPixmap，并按比例缩放到 QLabel 的大小
                    pLabel->setPixmap(QPixmap::fromImage(image).scaled(pLabel->size(), Qt::KeepAspectRatio));
                } else {
                    pLabel->setText("Invalid Image");
                }
            }
        });
    }
    // 设置克隆的 QLineEdit 大小与原始的一致
    else
        pMyProxy->resize(QSize(100, 20));

    // 添加克隆的 QLineEdit 到新的节点的 lineEdits 列表
    outputlabel.push_back(pLabel);
}

void QBlueprintNode::updateLabelWithData(QBlueprintPort* port, const QString& data) {
    auto it = std::find(inputPorts.begin(), inputPorts.end(), port);
    if (it != inputPorts.end()) {
        int index = std::distance(inputPorts.begin(), it) - 1;
        qDebug() << "outputlabel" << outputlabel.size() << "index" << index;
        if (index < outputlabel.size()) {
            QLabel* label = outputlabel[index];
            label->setText(data);
            scene()->update();  // 强制更新场景
        }
    }
}

void QBlueprintNode::processData(QBlueprintPort* inputPort, const QVariant& data) {
    // 根据 inputPort 来判断需要处理的逻辑
    qDebug() << "节点" << m_name << "接收到数据:" << data << "从端口:" << inputPort->name();
    // 如果有对应的 QLabel 需要更新（例如在 inputPort 上有 QLabel）
    auto it = std::find(inputPorts.begin(), inputPorts.end(), inputPort);
    if (it != inputPorts.end()) {
        int index = std::distance(inputPorts.begin(), it) - 1;
        if ((index < outputlabel.size()) /*&& isPortConnected(inputPort, outputPorts[index])*/) {
            QLabel* label = outputlabel[index];
            if (inputPort->portDataType() == DataType::QIMAGE && data.canConvert<QImage>()) {
                // 将 QVariant 数据转换为 QImage
                QImage image = data.value<QImage>();
                ImageLabel* imageLabel = dynamic_cast<ImageLabel*>(label); // 转化为ImageLabel
                // 将 QImage 转换为 QPixmap 并设置到 QLabel
                imageLabel->setImage(image);
                imageLabel->setPixmap(QPixmap::fromImage(image).scaled(label->size(), Qt::KeepAspectRatio));
            } else {
                // 更新 QLabel 显示的其他数据内容
                label->setText(data.toString());  // 更新 QLabel 显示的文本内容
            }
        }
    }
    QVariant result;
    if(class_name == "Math")
        result = mathFunctions();
#ifdef OPENCV_FOUND
    else if(class_name == "opencv")
        result = opencvFunctions();
#endif
    else if(class_name == "Qts")
        result = qtsFunctions();
    if(inputPort->getNodeType() == Type::FUNCTION)
        for (QBlueprintPort* outputPort : outputPorts) {
            if (outputPort) {
                outputPort->setVarType(result); // 更新输出端口的 var
                outputPort->sendDataToConnectedPorts(); // 发送数据到连接的下一个端口
            }
        }
    // 处理数据的逻辑，将数据传递给 outputPort
    else
        for (QBlueprintPort* outputPort : outputPorts) {
            if (outputPort && isPortConnected(inputPort, outputPort)) {
                qDebug() << "the data:" << data;
                outputPort->setVarType(data);  // 更新 outputPort 的数据
                outputPort->sendDataToConnectedPorts();  // 通过 outputPort 发送数据给下一个端口
            }
        }

}
bool QBlueprintNode::isPortConnected(QBlueprintPort* inputPort, QBlueprintPort* outputPort) {
    if(inputPort->getNodeType()==Type::FUNCTION && outputPort->getNodeType()==Type::FUNCTION)
        return true;
    auto inputIt = std::find(inputPorts.begin(), inputPorts.end(), inputPort);
    auto outputIt = std::find(outputPorts.begin(), outputPorts.end(), outputPort);

    if (inputIt == inputPorts.end() || outputIt == outputPorts.end()) {
        return false;
    }

    int inputIndex = std::distance(inputPorts.begin(), inputIt);
    int outputIndex = std::distance(outputPorts.begin(), outputIt);

    if (inputIndex != outputIndex) {
        return false;
    }

    // 获取场景和 Blueprint 视图
    QGraphicsScene* currentScene = scene();
    QBlueprint* blueprintView = dynamic_cast<QBlueprint*>(currentScene->views().first());

    if (blueprintView) {
        qDebug() << "sadas" <<blueprintView->isEventPortConnected(outputPort, inputPort);
        return blueprintView->isEventPortConnected(outputPort, inputPort);
    }
    return false;
}

QVariant QBlueprintNode::mathFunctions()
{
    QVariant result;
    if (m_name == "add" ) {
        // 加法操作
        result = Math::add(inputPorts[1]->data().toDouble(),inputPorts[2]->data().toDouble());
    }
    else if (m_name == "subtract" ) {
        // 减法操作
        result = Math::subtract(inputPorts[1]->data().toDouble(),inputPorts[2]->data().toDouble());

    }
    else if (m_name == "multiply" ) {
        // 乘法操作
        result = Math::multiply(inputPorts[1]->data().toDouble(),inputPorts[2]->data().toDouble());

    }
    else if (m_name == "divide" ) {
        // 除法操作，检查除数是否为零
        result = Math::divide(inputPorts[1]->data().toDouble(),inputPorts[2]->data().toDouble());

    }
    else if (m_name == "sqrt" ) {
        // 开方操作，只需要一个输入
        result = Math::sqrt(inputPorts[1]->data().toDouble());

    }
    else if (m_name == "pow") {
        result = Math::pow(inputPorts[1]->data().toDouble(),inputPorts[2]->data().toDouble());
    }
    else if (m_name == "deletea") {

    }
    return result;
}

QVariant QBlueprintNode::qtsFunctions()
{
    QVariant result;

    if (m_name == "setQPointF") {
        result = Qts::setQPointF(inputPorts[1]->data().toDouble(),
                                 inputPorts[2]->data().toDouble());
    } else if (m_name == "setQPoint") {
        result = Qts::setQPoint(inputPorts[1]->data().toInt(),
                                inputPorts[2]->data().toInt());
    } else if (m_name == "getQPointF_X") {
        QPointF point = inputPorts[1]->data().value<QPointF>();
        result = Qts::getQPointF_X(point);
    } else if (m_name == "getQPointF_Y") {
        QPointF point = inputPorts[1]->data().value<QPointF>();
        result = Qts::getQPointF_Y(point);
    } else if (m_name == "getQPoint_X") {
        QPoint point = inputPorts[1]->data().value<QPoint>();
        result = Qts::getQPoint_X(point);
    } else if (m_name == "getQPoint_Y") {
        QPoint point = inputPorts[1]->data().value<QPoint>();
        result = Qts::getQPoint_Y(point);
    }

    return result;
}

#ifdef OPENCV_FOUND
QVariant QBlueprintNode::opencvFunctions()
{
    QVariant result;
    if(m_name == "threshold")
        result = opencv::threshold(inputPorts[1]->data().value<QImage>(),
                                   inputPorts[2]->data().toDouble(),
                                   inputPorts[3]->data().toDouble(),
                                   inputPorts[4]->data().toInt());
    else if(m_name == "convertToGray")
    {
        result = opencv::convertToGray(inputPorts[1]->data().value<QImage>());
    }
    return result;
}
#endif









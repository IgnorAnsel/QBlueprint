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
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setZValue(1);
    dataType = datatype;
    setNodeType(Type);
    if(nodeType == Type::FUNCTION || nodeType == Type::BRANCH /*|| nodeType == Type::CONDITION*/ || nodeType == Type::FORLOOP)
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
        qDebug() << "yes";
        newNode->addInputPort(Type::FUNCTION); // 添加事件端口
        newNode->addOutputPort(Type::FUNCTION);
    }
    else if(newNode->nodeType == Type::BRANCH)
    {
        newNode->addInputPort(Type::BRANCH);
        newNode->addOutputPort(Type::BRANCH);
    }
    else if(newNode->nodeType == Type::CONDITION)
    {
        newNode->addButtonToTopLeft(Type::CONDITION);
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
    if(nodeType == Type::CONDITION)
        maxInputWidth = 57;
    int nodeWidth = maxInputWidth + maxOutputWidth + padding;
    int count = 0;
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
    else if(nodeType == Type::CONDITION)
    {
        nodeWidth += 50;
        for (size_t i = 0; i < inputPorts.size(); ++i) {
            if(inputPorts[i]->name() == "E1" || inputPorts[i]->name() == "E2")
                count ++;
            else if(inputPorts[i]->name() == "Condition")
                count +=2;
        }
        if(count > 2)
            nodeHeight = std::max(inputPorts.size(), outputPorts.size()) * 31 + 31 + (count/2 - 1) * 20;
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
void QBlueprintNode::addButtonToTopLeft(enum Type type)

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
        "    background-color: #2196F3;"  // 按钮背景颜色（蓝色）
        "    color: white;"  // 按钮文字颜色（白色）
        "    border: none;"  // 无边框
        "    border-radius: 0px;"  // 设置圆角半径，半径为按钮尺寸的一半
        "}"
        "QPushButton:pressed {"
        "    background-color: #45a049;"  // 点击时的背景颜色
        "}"
        );
    buttonProxy->setWidget(button);
    buttonProxy->setPos(25, 10);

    // 设置按钮的点击事件
    connect(button, &QPushButton::clicked, [this](){
        addInputPortCondition(Type::CONDITION);
    });
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
        else if(nodeType == Type::CONDITION) // 添加条件
        {
            addInputPort(Type::CONDITION);
            addOutputPort(Type::CONDITION);
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
void QBlueprintNode::addInputPortCondition(enum Type Type)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, "Condition", dataType, this, getEnumName(dataType));
    port->setNodeType(nodeType);
    setQVariantType(port);
    inputPorts.push_back(port);
    addOutputPort(Type);
    customNodePortSort();
    addOutputLabel(port, port);
    if(inputPorts.size()>1)
    {
        addRadioButtonOptions(port);
    }
    port->setVarType(QVariant::fromValue(bool()));
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
        QBlueprintPort *port_Condition = new QBlueprintPort(QBlueprintPort::Input, "Condition", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        port_Condition->setNodeType(nodeType);
        setQVariantType(port_Condition);
        inputPorts.push_back(port);
        inputPorts.push_back(port_Condition);
    }
    else if(Type == Type::CONDITION)
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, "E1", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        inputPorts.push_back(port);
        customNodePortSort();
        addOutputLabel(port, port);
        QBlueprintPort *port2 = new QBlueprintPort(QBlueprintPort::Input, "E2", dataType, this, getEnumName(dataType));
        port2->setNodeType(nodeType);
        setQVariantType(port2);
        inputPorts.push_back(port2);
        customNodePortSort();
        addOutputLabel(port2, port2);
        addLineEdit(port, port2);
        if(inputPorts.size()>2)
            addRadioButtonOptions(port);
        port->setVarType(QVariant::fromValue(int()));
        port2->setVarType(QVariant::fromValue(int()));

    }
    else
    {
        QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::EVENT_INPUT, "", dataType, this, getEnumName(dataType));
        port->setNodeType(nodeType);
        setQVariantType(port);
        inputPorts.push_back(port);
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
        QBlueprintPort *port_return_true = new QBlueprintPort(QBlueprintPort::EVENT_TRUE_RETURN, "True", dataType, this, getEnumName(dataType));
        QBlueprintPort *port_return_false = new QBlueprintPort(QBlueprintPort::EVENT_FALSE_RETURN, "False", dataType, this, getEnumName(dataType));
        port_return_true->setNodeType(nodeType);
        setQVariantType(port_return_true);
        port_return_false->setNodeType(nodeType);
        setQVariantType(port_return_false);
        outputPorts.push_back(port_return_true);
        outputPorts.push_back(port_return_false);
    }
    else if(Type == Type::CONDITION)
    {
        if(outputPorts.size()==0)
        {
            QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "Condition", dataType, this, getEnumName(dataType));
            port->setNodeType(nodeType);
            setQVariantType(port);
            port->setVarType(QVariant::fromValue(bool()));
            outputPorts.push_back(port);
        }
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
    if(nodeType == Type::CONDITION)
    {
        // 排列输入端口
        int count = 0;
        for (size_t i = 0; i < inputPorts.size(); ++i) {
            QFontMetrics fontMetrics(inputPorts[i]->m_font);
            int inputTextWidth = fontMetrics.horizontalAdvance(inputPorts[i]->name());
            inputPorts[i]->setPos(5, i * 30 + 40 + (count/2) * 20); // 左边距15，纵向位置
            if(inputPorts[i]->name() == "E1" || inputPorts[i]->name() == "E2")
                count ++;
            else if(inputPorts[i]->name() == "Condition")
                count +=2;
        }
        // 排列输出端口
        for (size_t i = 0; i < outputPorts.size(); ++i) {
            QFontMetrics fontMetrics(outputPorts[i]->m_font);
            int outputTextWidth = fontMetrics.horizontalAdvance(outputPorts[i]->name());
            outputPorts[i]->setPos(boundingRect().width() - 15, i * 60 + 40); // 右边距15
        }
    }
    else {
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
            if(nodeType == Type::CONDITION) // 条件节点的输出端口是需要间隔一个output的
                outputPorts[i]->setPos(boundingRect().width() - 15, i * 60 + 40); // 右边距15
            else
                outputPorts[i]->setPos(boundingRect().width() - 15, i * 30 + 40); // 右边距15
        }
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
void QBlueprintNode::addLineEdit(QBlueprintPort *port1, QBlueprintPort *port2)
{
    // 创建 QLineEdit
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("条件");  // 设置提示文本
    lineEdit->setStyleSheet("QLineEdit { border: 1px solid black; padding: 2px; }");  // 设置样式
    lineEdit->resize(80, 20);

    // 创建 QGraphicsProxyWidget 并将 lineEdit 添加到代理
    QGraphicsProxyWidget *pMyProxy = new QGraphicsProxyWidget(this);
    pMyProxy->setWidget(lineEdit);

    // 设置 Z 值，确保控件显示在前景
    pMyProxy->setZValue(10);

    // 设置代理的位置
    QPointF outputPortPos = port2->pos();
    pMyProxy->setPos(outputPortPos.x() - lineEdit->width() + 233, (outputPortPos.y()) - 3);

    // 将 lineEdit 添加到列表中
    lineEdits.push_back(lineEdit);
    relation.push_back("");  // 初始化为一个空的 QString，占据与该 lineEdit 对应的位置

    // 使用 QRegularExpressionValidator 限制输入条件符号
    QRegularExpression regex(R"(>|<|=|>=|<=|!=)");  // 允许的符号
    QValidator *validator = new QRegularExpressionValidator(regex, this);
    lineEdit->setValidator(validator);
    int currentIndex = lineEdits.size() - 1;  // 当前 lineEdit 的索引
    connect(lineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        QStringList allowedConditions = {">", "<", "=", ">=", "<=", "!="};
        if (allowedConditions.contains(text.trimmed())) {
            relation[currentIndex] = text.trimmed();  // 将输入的条件符号存入 relation 的对应位置
            qDebug() << "输入条件符号：" << relation[currentIndex];
        } else {
            qDebug() << "无效输入，当前仅支持条件符号：" << allowedConditions;
        }
    });
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
    //scene()->update();
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
    pLabel->setMinimumWidth(50);
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
    else if(inport->name() == "Condition")
        pMyProxy->resize(QSize(50, 20));
    else
        pMyProxy->resize(QSize(100, 20));

    // 添加克隆的 QLineEdit 到新的节点的 lineEdits 列表
    outputlabel.push_back(pLabel);
}
void QBlueprintNode::addRadioButtonOptions(QBlueprintPort *port)
{
    // 创建 QWidget 作为 QGraphicsProxyWidget 的容器
    QWidget *containerWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(containerWidget);  // 使用水平布局管理器

    // 设置布局的间距和边距
    layout->setSpacing(5);  // 控件之间的间距
    layout->setContentsMargins(0, 0, 0, 0);  // 去掉边距

    // 创建两个 QRadioButton 用于选择 || 和 &&
    QRadioButton *orOption = new QRadioButton("||");
    QRadioButton *andOption = new QRadioButton("&&");

    // 设置单选按钮的大小
    orOption->setFixedSize(40, 20);
    andOption->setFixedSize(40, 20);

    // 默认选中 || 选项
    orOption->setChecked(true);

    // 初始化 radioButtonOptions 和 radioButtonValues
    radioButtonOptions.push_back({orOption, andOption});
    radioButtonValues.push_back("||");  // 默认选中 "||"

    // 将单选按钮添加到布局
    layout->addWidget(orOption);
    layout->addWidget(andOption);

    // 创建 QGraphicsProxyWidget 并将 containerWidget 添加到代理
    QGraphicsProxyWidget *pMyProxy = new QGraphicsProxyWidget(this);
    pMyProxy->setWidget(containerWidget);

    // 设置代理的位置
    QPointF portPos = port->pos();
    pMyProxy->setPos(portPos.x(), portPos.y() - 30);  // 调整位置以适应布局

    // 确保布局适合代理的大小
    containerWidget->setLayout(layout);
    containerWidget->setFixedSize(235, 20);  // 缩小容器的大小

    int currentIndex = radioButtonOptions.size() - 1;  // 获取当前的索引
    qDebug() << "radioButtonValues:" <<radioButtonValues.size();
    // 连接单选按钮的信号到槽函数，用于处理选择的变化
    connect(orOption, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            radioButtonValues[currentIndex] = "||";  // 更新当前选项为 "||"
            qDebug() << "选择了 || 选项";
        }
    });

    connect(andOption, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            radioButtonValues[currentIndex] = "&&";  // 更新当前选项为 "&&"
            qDebug() << "选择了 && 选项";
        }
    });
}


void QBlueprintNode::processData(QBlueprintPort* inputPort, const QVariant& data) {
    if (nodeType == Type::CONDITION) // condition节点需要计算出bool值出来
    {
        QVariant result;
        bool result_bool;
        int count = 0;
        int radioButtonindex = 0;
        int relationindex = 0;
        for(int i = 0;i < inputPorts.size(); i++)
        {
            qDebug() << "inputPorts[i]->name()" << inputPorts[i]->name();
            if(inputPorts[i]->name() == "E1")
            {
                bool E_bool = false;
                QString relationSymbol = relation[relationindex]; // 获取当前的关系符号
                QVariant data1 = inputPorts[i]->data();
                QVariant data2 = inputPorts[i+1]->data();
                if (data1.canConvert<double>() && data2.canConvert<double>()) {
                    double value1 = data1.toDouble();
                    double value2 = data2.toDouble();
                    if (relationSymbol == ">") {
                        E_bool = value1 > value2;
                    } else if (relationSymbol == "<") {
                        E_bool = value1 < value2;
                    } else if (relationSymbol == "=") {
                        E_bool = value1 == value2;
                    } else if (relationSymbol == ">=") {
                        E_bool = value1 >= value2;
                    } else if (relationSymbol == "<=") {
                        E_bool = value1 <= value2;
                    } else if (relationSymbol == "!=") {
                        E_bool = value1 != value2;
                    }
                    else {
                        qDebug() << "Value1:" << value1 << "Value2:" << value2 << "Relation:" << relationSymbol << "E_bool:" << E_bool;
                        break;
                    }
                } else {
                    qDebug() << "无法比较非数值类型数据";
                }
                partbool.push_back(E_bool);
                relationindex++;
            }
            else if(inputPorts[i]->name() == "Condition")
            {
                QVariant data = inputPorts[i]->data();
                data.canConvert<bool>();
                bool condition = data.toBool();
                partbool.push_back(condition);
            }
            else
                continue;
        }
        qDebug() << "partbool.size()" << partbool.size();
        if(partbool.size() != 0)
            result_bool = partbool[0];
        for(int i = 0;i< partbool.size()-1;i++)
        {
            if(partbool.size() == 0)
                break;
            if(radioButtonValues[radioButtonindex] == "||")
                result_bool = result_bool || partbool[i+1];
            else if(radioButtonValues[radioButtonindex] == "&&")
                result_bool = result_bool && partbool[i+1];
            radioButtonindex ++;
        }
        result = result_bool;
        qDebug() << "result:" << result_bool;
    }
    // 根据 inputPort 来判断需要处理的逻辑
    qDebug() << "节点" << m_name << "接收到数据:" << data << "从端口:" << inputPort->name();
    // 如果有对应的 QLabel 需要更新（例如在 inputPort 上有 QLabel）
    auto it = std::find(inputPorts.begin(), inputPorts.end(), inputPort);
    if (it != inputPorts.end()) {
        int index = 0;
        if(nodeType == Type::CONDITION)
            index = std::distance(inputPorts.begin(), it);
        else
            index = std::distance(inputPorts.begin(), it) - 1;
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
void QBlueprintNode::addLabelToPort(QBlueprintPort* port, const QString &text)
{
    QLabel* label = new QLabel(text);
    label->setStyleSheet("QLabel { border: 1px solid black; padding: 2px; }");

    QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(label);

    QPointF portPos = port->pos();
    proxy->setPos(portPos.x() - 70, portPos.y() - 5);

    inputlabel.push_back(label);
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









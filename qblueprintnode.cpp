#include "qblueprintnode.h"

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
    //initData(dataType);
    initInputOrOutput(Type,datatype);
    setZValue(1);
    dataType = datatype;
    qDebug() << ":" << dataType;
    setNodeType(Type);
    if(nodeType == Type::FUNCTION)
        customNodePortSort();
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
    cleanData();
}
QBlueprintNode* QBlueprintNode::clone() const
{
    // 创建一个新的 QBlueprintNode 实例
    QBlueprintNode* newNode = new QBlueprintNode(this->nodeType,this->dataType);
    newNode->setNodeTitle(this->m_name);
    newNode->setClassName(this->class_name);
    newNode->setNodeType(this->nodeType);
    // 克隆输入端口
    for (QBlueprintPort* port : this->inputPorts) {
        QBlueprintPort* clonedPort = port->clone(); // 假设 QBlueprintPort 有一个 clone 方法
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->inputPorts.push_back(clonedPort);
    }

    // 克隆输出端口
    for (size_t i = 0; i < this->outputPorts.size(); ++i) {
        QBlueprintPort* port = this->outputPorts[i];
        QBlueprintPort* clonedPort = port->clone(); // 假设 QBlueprintPort 有一个 clone 方法
        clonedPort->setParentItem(newNode); // 设置父项为新的 QBlueprintNode
        newNode->outputPorts.push_back(clonedPort);

        // 确保 LineEdit 与 OutputPort 对应
        if (i < this->lineEdits.size()) {
            QLineEdit* lineEdit = this->lineEdits[i];

            // 创建新的 QLineEdit 实例，复制现有的文本内容
            QLineEdit* clonedLineEdit = new QLineEdit(lineEdit->text());
            clonedLineEdit->setStyleSheet("QLineEdit { border: 1px solid black; border-radius: 0px; padding: 2px; }");

            // 创建新的 QGraphicsProxyWidget 并将克隆的 QLineEdit 嵌入其中
            QGraphicsProxyWidget* clonedProxy = new QGraphicsProxyWidget(newNode); // 父项是新的 QBlueprintNode
            clonedProxy->setWidget(clonedLineEdit);  // 将新的 QLineEdit 嵌入到新的代理控件中

            QPointF outputPortPos = port->pos();
            clonedProxy->setPos(outputPortPos.x() - clonedLineEdit->width() + 210, outputPortPos.y() + 35 + i * 30);

            // 设置克隆的 QLineEdit 大小与原始的一致
            clonedProxy->resize(QSize(60, 10));

            // 添加克隆的 QLineEdit 到新的节点的 lineEdits 列表
            newNode->lineEdits.push_back(clonedLineEdit);
        }
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

    // 如果是 QIMAGE 类型节点，增加宽度和高度
    if (dataType == DataType::QIMAGE)
    {
        nodeWidth += 110;
        nodeHeight += outputPorts.size() * 90;
    }
    if (dataType == DataType::INT)
    {
        int maxLineEditWidth = 0;
        for(const auto& lineEdit : lineEdits)
        {
            if(maxLineEditWidth < lineEdit->width())
                maxLineEditWidth = lineEdit->width();
        }
        nodeWidth += maxLineEditWidth;
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
            case DataType::INT:{
                addLineEdit(addOutputPort(getEnumName(dataType)));
                break;
            }
            case DataType::QIMAGE:{
                QBlueprintPort * port = addOutputPort(getEnumName(dataType));
                imageNodePortSort();
                addInputLabel(outputPorts[outputPorts.size()-1]);
                break;
            }
            default:
                break;
            }

        }
        else if(nodeType == Type::OUTPUT){
            switch (dataType) {
            case DataType::INT:{
                QBlueprintPort * outputport = addOutputPort(getEnumName(dataType));
                QBlueprintPort * inputport = addInputPort(getEnumName(dataType));
                customNodePortSort();
                addOutputLabel(outputport,inputport);
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

        qDebug() << "Button clicked!";
    });
}




QBlueprintPort* QBlueprintNode::addInputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, "NULL", this);
    port->setNodeType(nodeType);
    inputPorts.push_back(port);
    return port;
}

QBlueprintPort* QBlueprintNode::addOutputPort()
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, "NULL", this);
    port->setNodeType(nodeType);
    outputPorts.push_back(port);
    return port;
}

QBlueprintPort* QBlueprintNode::addInputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Input, name, this);
    port->setNodeType(nodeType);
    qDebug() << "input" << nodeType << port->getNodeType();
    inputPorts.push_back(port);
}

QBlueprintPort* QBlueprintNode::addOutputPort(const QString &name)
{
    QBlueprintPort *port = new QBlueprintPort(QBlueprintPort::Output, name, this);
    port->setNodeType(nodeType);
    outputPorts.push_back(port);
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
    for (size_t i = 0; i < inputPorts.size(); ++i) {
        // 将输入端口放在左侧
        inputPorts[i]->setPos(5, i * 115 + 35);  // 左边距
    }

    for (size_t i = 0; i < outputPorts.size(); ++i) {
        // 将输出端口放在右侧
        outputPorts[i]->setPos(boundingRect().width() - 15, i * 115 + 35); // 右边距
    }
}


void QBlueprintNode::initInputOrOutput(enum Type Type, DataType datatype)
{
    if(Type == Type::INPUT)
    {
        switch(datatype){
        case DataType::INT:
            break;
        }
    }
    else if(Type == Type::OUTPUT)
    {
        switch(datatype){
        case DataType::INT:
            break;
        case FOR_FUNCTION:
        case FLOAT:
        case DOUBLE:
        case CHAR:
        case STRING:
        case BOOL:
        case LONG:
        case SHORT:
        case UNSIGNED_INT:
        case VARIANT:
        case QSTRING:
        case QDATE:
        case QDATETIME:
        case QTIME:
        case QPOINT:
        case QPOINTF:
        case QSIZE:
        case QSIZEF:
        case QRECT:
        case QRECTF:
        case QCOLOR:
        case QPIXMAP:
        case QIMAGE:
        case QPEN:
        case QBRUSH:
        case QFONT:
            break;
        }
    }
}
// enum DataType {
//     FOR_FUNCTION,          // 为FUNCTION使用
//     INT,           // 整型
//     FLOAT,         // 单精度浮点型
//     DOUBLE,        // 双精度浮点型
//     CHAR,          // 字符型
//     STRING,        // 字符串型
//     BOOL,          // 布尔型
//     LONG,          // 长整型
//     SHORT,         // 短整型
//     UNSIGNED_INT,  // 无符号整型
//     VARIANT,       // QVariant 类型 (Qt通用类型)
//     QSTRING,       // QString 类型 (Qt字符串类型)
//     QDATE,         // QDate 类型 (Qt日期类型)
//     QDATETIME,     // QDateTime 类型 (Qt日期时间类型)
//     QTIME,         // QTime 类型 (Qt时间类型)
//     QPOINT,        // QPoint 类型 (Qt坐标点类型)
//     QPOINTF,       // QPointF 类型 (Qt浮点坐标点类型)
//     QSIZE,         // QSize 类型 (Qt尺寸类型)
//     QSIZEF,        // QSizeF 类型 (Qt浮点尺寸类型)
//     QRECT,         // QRect 类型 (Qt矩形类型)
//     QRECTF,        // QRectF 类型 (Qt浮点矩形类型)
//     QCOLOR,        // QColor 类型 (Qt颜色类型)
//     QPIXMAP,       // QPixmap 类型 (Qt图像类型)
//     QIMAGE,        // QImage 类型 (Qt图像类型)
//     QPEN,          // QPen 类型 (Qt画笔类型)
//     QBRUSH,        // QBrush 类型 (Qt画刷类型)
//     QFONT          // QFont 类型 (Qt字体类型)
// };
void QBlueprintNode::initData(DataType datatype)
{
    switch(datatype){
    case DataType::INT:
        intData = new std::vector<int>();
        break;
    case DataType::FLOAT:
        floatData = new std::vector<float>();
        break;
    case DataType::DOUBLE:
        doubleData = new std::vector<double>();
        break;
    case DataType::CHAR:
        charData = new std::vector<char>();
        break;
    case DataType::STRING:
        stringData = new std::vector<std::string>();
        break;
    case DataType::BOOL:
        boolData = new std::vector<bool>();
        break;
    case DataType::LONG:
        longData = new std::vector<long>();
        break;
    case DataType::SHORT:
        shortData = new std::vector<short>();
        break;
    case DataType::UNSIGNED_INT:
        unsignedIntData = new std::vector<unsigned int>();
        break;
    case DataType::VARIANT:
        variantData = new std::vector<QVariant>();
        break;
    case DataType::QSTRING:
        qStringData = new std::vector<QString>();
        break;
    case DataType::QDATE:
        qDateData = new std::vector<QDate>();
        break;
    // case :
    //     = new std::vector<>();
    //     break;
    // case :
    //     = new std::vector<>();
    //     break;
    // case :
    //     = new std::vector<>();
    //     break;
    // case :
    //     = new std::vector<>();
    //     break;
    default:
        qDebug() << "unkown type:" << datatype;
    }
}

void QBlueprintNode::cleanData()
{
    switch (dataType) {
    case DataType::INT:
        delete intData;
        break;
    default:
        break;
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

    // 设置克隆的 QLineEdit 大小与原始的一致
    pMyProxy->resize(QSize(60, 10));

    // 添加克隆的 QLineEdit 到新的节点的 lineEdits 列表
    lineEdits.push_back(pLineEdit);
}
void QBlueprintNode::adjustLineEditWidth(const QString &text) {
    // 创建 QFont 对象
    QFont font; // 使用默认字体，或者根据需要设置特定的字体
    QFontMetrics fontMetrics(font);

    // 计算文本宽度
    int textWidth = fontMetrics.horizontalAdvance(text);

    // 设置 QLineEdit 的宽度，添加一些额外的边距
    for (auto lineEdit : lineEdits) {  // 假设 lineEdits 是存储所有 QLineEdit 的列表
        lineEdit->setFixedWidth(textWidth + 20);  // 添加20像素的边距
    }
}

void QBlueprintNode::addInputLabel(QBlueprintPort* port)
{

    // 创建 QWidget 作为 QGraphicsProxyWidget 的容器
    QWidget* containerWidget = new QWidget();
    containerWidget->setFixedSize(150, 110); // 设置容器大小，可以调整为合适的大小

    // 创建 QLabel 用于显示图片
    QLabel* pLabel = new QLabel(containerWidget); // 直接设置父对象为 containerWidget
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
        }
    });

    // 确保场景更新
    scene()->update();
}


void QBlueprintNode::addOutputLabel(QBlueprintPort *outport, QBlueprintPort *inport)
{
    // 创建 QLabel
    QLabel* pLabel = new QLabel("");
    pLabel->setStyleSheet("QLabel { border: 1px solid black; border-radius: 0px; padding: 2px; }");

    // 创建 QGraphicsProxyWidget 容器
    QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
    pMyProxy->setWidget(pLabel);

    // 动态计算宽度：根据输入端口名称、输出端口名称以及 label 的宽度来计算
    QFontMetrics fontMetrics(pLabel->font());
    int inputPortTextWidth = fontMetrics.horizontalAdvance(inport->name()); // 输入端口名称宽度
    int outputPortTextWidth = fontMetrics.horizontalAdvance(outport->name()); // 输出端口名称宽度

    // 设置 label 的位置，label 在输入端口和输出端口之间
    int labelPadding = 20; // 元素之间的间隔
    int totalWidth = inputPortTextWidth + outputPortTextWidth + pLabel->width() + (2 * labelPadding);

    // 获取端口位置
    QPointF inputPortPos = inport->pos();
    QPointF outputPortPos = outport->pos();

    // 将 label 放在合适位置，居中显示
    pMyProxy->setPos(inputPortPos.x() + inputPortTextWidth + labelPadding, inputPortPos.y());

    // 设置 label 的大小
    pMyProxy->resize(pLabel->sizeHint());

    // 保存 label 到列表
    outputlabel.push_back(pLabel);

    // 重新计算节点边界
    scene()->update();
}





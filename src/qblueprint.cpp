#include "qblueprint.h"
#include <unordered_map>
#include <QWidgetAction>
#include "clickableframe.h"

QBlueprint::QBlueprint(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this))  // 初始化场景
{
    // 设置场景的范围，可以根据需要调整
    scene->setSceneRect(0, 0, 8000, 8000);
#ifdef OPENCV_FOUND
    qDebug() << "OPENCV FOUND";
#endif
    // 将场景设置为QGraphicsView的场景
    setScene(scene);

    // 可选：设置视图的一些属性，比如抗锯齿等
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);  // 设置拖拽模式
    // 初始视图缩放
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setBackgroundBrush(QColor(30, 30, 30));  // 设置深色背景
    createBlueprintNodes();
}

void QBlueprint::createBlueprintNodes() // 使用工厂方法基于函数生成节点
{

    createOutputNode();
    createInputNode();
    createControlNode();
    // 自动获取函数名不正常，直接填写你需要的名称

    // 创建 Math 相关的运算节点
    QBlueprintNode* math_add_node = QNodeFactory::createNodeFromFunction(this, &Math::add, "add", "Math");
    QBlueprintNode* math_subtract_node = QNodeFactory::createNodeFromFunction(this, &Math::subtract, "subtract", "Math");
    QBlueprintNode* math_multiply_node = QNodeFactory::createNodeFromFunction(this, &Math::multiply, "multiply", "Math");
    QBlueprintNode* math_divide_node = QNodeFactory::createNodeFromFunction(this, &Math::divide, "divide", "Math");
    QBlueprintNode* math_sqrt_node = QNodeFactory::createNodeFromFunction(this, &Math::sqrt, "sqrt", "Math");
    QBlueprintNode* math_pow_node = QNodeFactory::createNodeFromFunction(this, &Math::pow, "pow", "Math");
    // 创建 Qt 一些难以实现的(QPoint...)
    QBlueprintNode* qts_setpointf_node = QNodeFactory::createNodeFromFunction(this, &Qts::setQPointF,Qts::inputNames_setQPointF,Qts::outputName_setQPointF, "setQPointF", "Qts");
    QBlueprintNode* qts_setpoint_node = QNodeFactory::createNodeFromFunction(this, &Qts::setQPoint, "setQPoint", "Qts");
    QBlueprintNode* qts_getpointf_x_node = QNodeFactory::createNodeFromFunction(this, &Qts::getQPointF_X, "getQPointF_X", "Qts");
    QBlueprintNode* qts_getpointf_y_node = QNodeFactory::createNodeFromFunction(this, &Qts::getQPointF_Y, "getQPointF_Y", "Qts");
    QBlueprintNode* qts_getpoint_x_node = QNodeFactory::createNodeFromFunction(this, &Qts::getQPoint_X, "getQPoint_X", "Qts");
    QBlueprintNode* qts_getpoint_y_node = QNodeFactory::createNodeFromFunction(this, &Qts::getQPoint_Y, "getQPoint_Y", "Qts");

#ifdef OPENCV_FOUND
    QBlueprintNode* opencv_threshold_node = QNodeFactory::createNodeFromFunction(this, &opencv::threshold, opencv::inputNames_threshold,opencv::outputName_threshold,"threshold", "opencv");
    QBlueprintNode* opencv_BGRtoGRAY_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::BGRtoGRAY,
        opencv::inputNames_BGRtoGRAY,
        opencv::outputName_BGRtoGRAY,
        "BGRtoGRAY",
        "opencv");
    QBlueprintNode* opencv_BGRtoHSV_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::BGRtoHSV,
        opencv::inputNames_BGRtoHSV,
        opencv::outputName_BGRtoHSV,
        "BGRtoHSV",
        "opencv");
    QBlueprintNode* opencv_erode_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::erode,
        opencv::inputNames_erode,
        opencv::outputName_erode,
        "erode",
        "opencv"
        );
    QBlueprintNode* opencv_dilate_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::dilate,
        opencv::inputNames_dilate,
        opencv::outputName_dilate,
        "dilate",
        "opencv"
        );
    QBlueprintNode* opencv_canny_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::canny,
        opencv::inputNames_canny,
        opencv::outputName_canny,
        "canny",
        "opencv"
        );
    QBlueprintNode* opencv_gaussianblur_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::gaussianblur,
        opencv::inputNames_gaussianblur,
        opencv::outputName_gaussianblur,
        "gaussianblur",
        "opencv"
        );
    QBlueprintNode* opencv_rotateandincline_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::rotateandincline,
        opencv::inputNames_rotateandincline,
        opencv::outputName_rotateandincline,
        "rotateandincline",
        "opencv"
        );
    QBlueprintNode* opencv_per_trans_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::per_trans,
        opencv::inputNames_per_trans,
        opencv::outputName_per_trans,
        "per_trans",
        "opencv"
        );
    QBlueprintNode* opencv_resize_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::resize,
        opencv::inputNames_resize,
        opencv::outputName_resize,
        "resize",
        "opencv"
        );
    QBlueprintNode* opencv_line_node = QNodeFactory::createNodeFromFunction(
        this,
        &opencv::line,
        opencv::inputNames_line,
        opencv::outputName_Bline,
        "line",
        "opencv"
        );

    //QBlueprintNode* opencv_convertToGray_node = QNodeFactory::createNodeFromFunction(this, &opencv::convertToGray, "convertToGray", "opencv");
#endif
    classifyNodes();
}

void QBlueprint::classifyNodes()
{
    std::unordered_map<QString, std::vector<QString>> classifiedNodes;
    for (QBlueprintNode* node : save_nodes) {
        QString className = node->getClassName();
        QString functionName = node->getNodeTitle();

        // 将函数名添加到对应类名的列表中
        classifiedNodes[className].push_back(functionName);
    }

    // 输出分类结果，或者进一步处理
    for (const auto& entry : classifiedNodes) {
        QString className = entry.first;
        const std::vector<QString>& functions = entry.second;

        qDebug() << "Class Name:" << className;
        for (const QString& func : functions) {
            qDebug() << "  Function:" << func;
        }
    }
}
QBlueprint::~QBlueprint()
{

}
void QBlueprint::contextMenuEvent(QContextMenuEvent* event)
{
    // 创建右键菜单
    QMenu contextMenu;
    QPointF scenePos = mapToScene(event->pos());
    m_menuActions.clear(); // 清除之前的菜单项缓存

    // 设置样式表
    contextMenu.setStyleSheet(
        "QMenu { "
        "   background-color: #353535; "
        "   color: white; "
        "   border: 2px solid #5A5A5A; "
        "   border-radius: 8px; "
        "} "
        "QMenu::item:selected { "
        "   background-color: #2A82DA; "
        "   color: white; "
        "   border-radius: 4px; "
        "}"
        );

    // 添加搜索框
    QWidgetAction* searchAction = new QWidgetAction(&contextMenu);
    QFrame* searchFrame = new QFrame();
    QHBoxLayout* searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(5, 5, 5, 5);

    m_searchEdit = new QLineEdit(searchFrame);
    m_searchEdit->setPlaceholderText("Search...");
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setText(m_searchText); // 设置当前搜索文本
    m_searchEdit->setStyleSheet(
        "QLineEdit {"
        "   background-color: #454545;"
        "   color: white;"
        "   border: 1px solid #5A5A5A;"
        "   border-radius: 4px;"
        "   padding: 3px;"
        "}"
        "QLineEdit:focus {"
        "   border: 1px solid #2A82DA;"
        "}"
    );
    m_searchEdit->installEventFilter(this);
    searchLayout->addWidget(m_searchEdit);
    searchFrame->setLayout(searchLayout);
    searchAction->setDefaultWidget(searchFrame);
    contextMenu.addAction(searchAction);

    // 连接搜索框的信号 - 使用延迟过滤
    connect(m_searchEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        m_searchText = text;
        filterMenuItems();
    });

    // 添加收藏夹菜单
    QMenu* favoritesMenu = contextMenu.addMenu("★ Favorites");
    favoritesMenu->setStyleSheet(
        "QMenu { "
        "   background-color: #FFD700; "  // 金色背景
        "   color: black; "
        "   border: 2px solid #FFA500; "   // 橙色边框
        "   border-radius: 8px; "
        "} "
        "QMenu::item:selected { "
        "   background-color: #FFA500; "   // 橙色悬停
        "   color: black; "
        "   border-radius: 4px; "
        "}"
        );

    // 添加收藏的节点
    for (QBlueprintNode* node : m_favoriteNodes) {
        addNodeToMenu(favoritesMenu, node, scenePos);
    }

    // 分组节点
    QMap<QString, QList<QBlueprintNode*>> classNodeMap;
    for (QBlueprintNode* node : save_nodes) {
        classNodeMap[node->getClassName()].append(node);
    }

    // 创建分类菜单
    for (auto it = classNodeMap.begin(); it != classNodeMap.end(); ++it) {
        QString className = it.key();
        QList<QBlueprintNode*> nodes = it.value();

        QMenu* classMenu = contextMenu.addMenu(className);

        // 设置分类样式
        if (className == "Input") {
            classMenu->setStyleSheet(
            "QMenu { "
                           "   background-color: #DFFFD6; "  // 二级菜单的绿色背景
                           "   color: black; "               // 二级菜单的文字颜色
                           "   border: 2px solid green; "     // 二级菜单的绿色边框
                           "   border-radius: 8px; "          // 圆角边框
                           "} "
                           "QMenu::item:selected { "
                           "   background-color: #8CD98C; "  // 鼠标悬停时的绿色背景
                           "   color: black; "               // 鼠标悬停时的文字颜色
                           "   border-radius: 4px; "          // 圆角边框
                           "}"
                           );
        } else if (className == "Output") {
            classMenu->setStyleSheet(
            "QMenu { "
                            "   background-color: #FFD6D6; "  // 二级菜单的红色背景
                            "   color: black; "               // 二级菜单的文字颜色
                            "   border: 2px solid red; "       // 二级菜单的红色边框
                            "   border-radius: 8px; "          // 圆角边框
                            "} "
                            "QMenu::item:selected { "
                            "   background-color: #FF7A7A; "  // 鼠标悬停时的红色背景
                            "   color: black; "               // 鼠标悬停时的文字颜色
                            "   border-radius: 4px; "          // 圆角边框
                            "}");
        } else {
            classMenu->setStyleSheet(
            "QMenu { "
                            "   background-color: #D6E6FF; "  // 二级菜单的蓝色背景
                            "   color: black; "               // 二级菜单的文字颜色
                            "   border: 2px solid blue; "      // 二级菜单的蓝色边框
                            "   border-radius: 8px; "          // 圆角边框
                            "} "
                            "QMenu::item:selected { "
                            "   background-color: #7A9EFF; "  // 鼠标悬停时的蓝色背景
                            "   color: black; "               // 鼠标悬停时的文字颜色
                            "   border-radius: 4px; "          // 圆角边框
                            "}");
        }

        // 添加节点项
        for (QBlueprintNode* node : nodes) {
            addNodeToMenu(classMenu, node, scenePos);
        }
    }

    // 保存菜单指针以便后续刷新
    m_currentContextMenu = &contextMenu;
    m_currentEventPos = event->globalPos();

    // 初始过滤
    filterMenuItems();

    contextMenu.exec(event->globalPos());

    // 菜单关闭后清除指针
    m_currentContextMenu = nullptr;
    m_searchEdit = nullptr;
    m_menuActions.clear();
}

// 添加辅助函数判断节点是否应该显示
bool QBlueprint::shouldShowNode(QBlueprintNode* node, const QString& searchText)
{
    if (searchText.isEmpty()) return true;

    QString nodeTitle = node->getNodeTitle();
    QString className = node->getClassName();

    return nodeTitle.contains(searchText, Qt::CaseInsensitive) ||
           className.contains(searchText, Qt::CaseInsensitive);
}

// 过滤菜单项
void QBlueprint::filterMenuItems()
{
    if (!m_currentContextMenu) return;

    // 遍历所有菜单和菜单项
    for (auto menuIt = m_menuActions.begin(); menuIt != m_menuActions.end(); ++menuIt) {
        QMenu* menu = menuIt.key();
        QList<QWidgetAction*> actions = menuIt.value();

        // 检查该菜单是否有可见项
        bool hasVisibleItems = false;

        for (QWidgetAction* action : actions) {
            ClickableFrame* frame = qobject_cast<ClickableFrame*>(action->defaultWidget());
            if (!frame) continue;

            // 获取节点指针
            QVariant nodeVar = frame->property("nodePtr");
            if (!nodeVar.isValid()) continue;

            QBlueprintNode* node = nodeVar.value<QBlueprintNode*>();
            if (!node) continue;

            // 根据搜索文本决定是否显示
            bool visible = shouldShowNode(node, m_searchText);
            action->setVisible(visible);

            if (visible) {
                hasVisibleItems = true;
            }
        }

        // 设置菜单可见性
        menu->menuAction()->setVisible(hasVisibleItems);
    }

    // 更新菜单布局
    m_currentContextMenu->adjustSize();
}

// 修改后的 addNodeToMenu 函数
void QBlueprint::addNodeToMenu(QMenu* menu, QBlueprintNode* node, const QPointF& scenePos)
{
    // 创建自定义菜单项
    QWidgetAction* widgetAction = new QWidgetAction(menu);
    ClickableFrame* frame = new ClickableFrame();
    frame->setObjectName("menuItemFrame");
    frame->setProperty("nodePtr", QVariant::fromValue(node)); // 存储节点指针

    QHBoxLayout* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(5, 2, 5, 2);

    // 节点名称标签
    QLabel* label = new QLabel(node->getNodeTitle(), frame);
    label->setStyleSheet("color: black; background: transparent;");
    layout->addWidget(label);

    QLabel* starLabel = new QLabel(frame);
    starLabel->setAlignment(Qt::AlignCenter);
    starLabel->setFixedSize(20, 20);
    starLabel->setStyleSheet("color: gold; font-weight: bold; background: transparent;");
    starLabel->setText(m_favoriteNodes.contains(node) ? "★" : "☆");
    starLabel->setProperty("nodePtr", QVariant::fromValue(node)); // 存储节点指针

    // 使星标可点击
    starLabel->setCursor(Qt::PointingHandCursor);
    starLabel->installEventFilter(this);

    layout->addWidget(starLabel);

    frame->setLayout(layout);
    widgetAction->setDefaultWidget(frame);
    menu->addAction(widgetAction);

    // 存储菜单项以便过滤
    if (!m_menuActions.contains(menu)) {
        m_menuActions[menu] = QList<QWidgetAction*>();
    }
    m_menuActions[menu].append(widgetAction);

    // 设置frame的样式表
    frame->setStyleSheet(
        "ClickableFrame#menuItemFrame {"
        "   background-color: transparent;"
        "   border-radius: 4px;"
        "}"
        "ClickableFrame#menuItemFrame:hover {"
        "   background-color: #2A82DA;"
        "}"
    );

    // 节点点击事件
    connect(frame, &ClickableFrame::clicked, [this, node, scenePos]() {
        placeNodeInScene(node, scenePos);
    });

    // 星标点击事件
    connect(starLabel, &QLabel::linkActivated, [this, node]() {
        toggleFavorite(node);
    });
}

// 事件过滤器
bool QBlueprint::eventFilter(QObject* obj, QEvent* event)
{
    // 处理星标点击
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* starLabel = qobject_cast<QLabel*>(obj);
        if (starLabel) {
            // 标记正在点击星标
            m_isClickingStar = true;

            // 获取存储的节点指针
            QVariant nodeVar = starLabel->property("nodePtr");
            if (nodeVar.isValid()) {
                QBlueprintNode* node = nodeVar.value<QBlueprintNode*>();
                if (node) {
                    toggleFavorite(node);
                    return true;
                }
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        // 星标点击完成
        if (m_isClickingStar) {
            m_isClickingStar = false;
        }
    }

    // 处理搜索框点击
    if (obj == m_searchEdit) {
        if (event->type() == QEvent::MouseButtonPress) {
            // 标记正在点击搜索框
            m_isClickingSearch = true;

            // 阻止菜单关闭
            return true;
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            // 搜索框点击完成
            m_isClickingSearch = false;
        }
        else if (event->type() == QEvent::FocusOut) {
            // 当搜索框失去焦点时，如果不是因为点击其他地方，则恢复焦点
            if (!m_isClickingStar && !m_isClickingSearch) {
                m_searchEdit->setFocus();
                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

// 切换收藏状态
void QBlueprint::toggleFavorite(QBlueprintNode* node)
{
    if (m_favoriteNodes.contains(node)) {
        m_favoriteNodes.removeOne(node);
    } else {
        m_favoriteNodes.append(node);
    }

    // 刷新当前打开的菜单
    if (m_currentContextMenu) {
        // 关闭当前菜单
        m_currentContextMenu->close();

        // 延迟重新打开菜单
        QTimer::singleShot(50, this, [this]() {
            if (m_currentEventPos.isNull()) return;

            // 创建新的右键菜单事件
            QContextMenuEvent event(QContextMenuEvent::Mouse,
                                   mapFromGlobal(m_currentEventPos),
                                   m_currentEventPos);

            // 调用右键菜单处理函数
            contextMenuEvent(&event);
        });
    }
}
void QBlueprint::placeNodeInScene(QBlueprintNode* originalNode, const QPointF& mousePos)
{
    // 使用 clone 方法创建节点的副本
    QBlueprintNode* newNode = originalNode->clone();
    // 设置位置
    newNode->setPos(mousePos);

    // 将节点添加到场景
    scene->addItem(newNode);

    // 将节点添加到 scene_nodes 向量中
    scene_nodes.push_back(newNode);
}


void QBlueprint::pushVectorQBlueprintNode(QBlueprintNode *node)
{
    save_nodes.push_back(node);
}

// 重载绘制背景方法
void QBlueprint::drawBackground(QPainter *painter, const QRectF &rect)
{
    // 调用父类的绘制背景方法（以便保持默认行为）
    QGraphicsView::drawBackground(painter, rect);

    // 绘制网格背景
    const int gridSize = 20;  // 网格大小
    QPen lightPen(QColor(60, 60, 60), 1);  // 浅色网格线
    QPen darkPen(QColor(25, 25, 25), 2);   // 深色网格线

    // 获取当前视图的矩形区域
    QRectF viewRect = rect;

    // 绘制小网格线
    painter->setPen(lightPen);
    for (qreal x = std::floor(viewRect.left() / gridSize) * gridSize; x < viewRect.right(); x += gridSize)
    {
        painter->drawLine(QLineF(x, viewRect.top(), x, viewRect.bottom()));
    }
    for (qreal y = std::floor(viewRect.top() / gridSize) * gridSize; y < viewRect.bottom(); y += gridSize)
    {
        painter->drawLine(QLineF(viewRect.left(), y, viewRect.right(), y));
    }

    // 绘制大网格线（每隔5个小网格）
    painter->setPen(darkPen);
    for (qreal x = std::floor(viewRect.left() / (gridSize * 5)) * (gridSize * 5); x < viewRect.right(); x += gridSize * 5)
    {
        painter->drawLine(QLineF(x, viewRect.top(), x, viewRect.bottom()));
    }
    for (qreal y = std::floor(viewRect.top() / (gridSize * 5)) * (gridSize * 5); y < viewRect.bottom(); y += gridSize * 5)
    {
        painter->drawLine(QLineF(viewRect.left(), y, viewRect.right(), y));
    }
}

void QBlueprint::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;  // 缩放因子

    // 获取当前视图的变换矩阵
    QTransform currentTransform = transform();

    // 当前的缩放比例，假设 x 和 y 的缩放比例相同
    double currentScale = currentTransform.m11();  // m11() 获取水平缩放比例

    // 根据滚轮滚动方向进行缩放，并限制缩放范围
    if (event->angleDelta().y() > 0)  // 如果滚轮向上滚动，试图放大
    {
        if (currentScale < maxScaleFactor)  // 限制最大缩放
        {
            scale(scaleFactor, scaleFactor);  // 放大视图
        }
    }
    else  // 如果滚轮向下滚动，试图缩小
    {
        if (currentScale > minScaleFactor)  // 限制最小缩放
        {
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // 缩小视图
        }
    }

    // 获取鼠标在视图中的位置
    QPointF viewPos = event->position();

    // 将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(viewPos.toPoint());

    // 获取缩放后的鼠标位置在场景中的位置
    QPointF newScenePos = mapToScene(viewPos.toPoint());

    // 计算视图需要移动的距离，以使得缩放后鼠标位置不变
    QPointF delta = newScenePos - scenePos;

    // 调整视图的中心，使得缩放后的鼠标位置保持不变
    setTransformationAnchor(QGraphicsView::NoAnchor);
    translate(delta.x(), delta.y());
    // 在缩放后更新所有连接的位置
    updateAllConnections();
    event->accept();  // 接受事件
}
void QBlueprint::mousePressEvent(QMouseEvent *event)
{
    // 将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(event->pos());

    // 查找鼠标点击的图元
    QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
    qDebug()<< "clicked item:" << item;
    // 首先检查是否点击在端口上
    QBlueprintPort *port = dynamic_cast<QBlueprintPort*>(item);
    if (port)
    {
        qDebug() << "Clicked on port:" << port->name();
        m_draggingPort = port;
        startConnectionDrag(scenePos);
        return;
    }

    // 然后检查是否点击在节点上
    QBlueprintNode *node = dynamic_cast<QBlueprintNode*>(item);
    if (node)
    {
        // 使用访问器方法获取端口列表
        const auto& inputPorts = node->getInputPorts();
        const auto& outputPorts = node->getOutputPorts();

        // 遍历找到点击的端口
        for (auto *port : inputPorts)
        {
            if (port->contains(port->mapFromScene(scenePos)))
            {
                qDebug() << "Clicked on input port:" << port->name();
                m_draggingPort = port;
                startConnectionDrag(scenePos);
                return;
            }
        }

        for (auto *port : outputPorts)
        {
            if (port->contains(port->mapFromScene(scenePos)))
            {
                qDebug() << "Clicked on output port:" << port->name();
                m_draggingPort = port;
                startConnectionDrag(scenePos);
                return;
            }
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void QBlueprint::mouseMoveEvent(QMouseEvent *event)
{
    if (m_draggingPort && m_currentConnection)
    {
        // 将视图坐标转换为场景坐标
        QPointF scenePos = mapToScene(event->pos());

        // 更新临时连线的位置
        m_currentConnection->updatePosition(m_draggingPort->centerPos(), scenePos);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void QBlueprint::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_draggingPort && m_currentConnection)
    {
        // 将视图坐标转换为场景坐标
        QPointF scenePos = mapToScene(event->pos());

        // 遍历场景中的所有项目，寻找匹配的端口
        QBlueprintPort *targetPort = nullptr;
        for (QGraphicsItem *item : scene->items(scenePos))
        {
            targetPort = dynamic_cast<QBlueprintPort*>(item);
            if (targetPort && targetPort != m_draggingPort && targetPort->portType() != m_draggingPort->portType()&&targetPort->parentItem() != m_draggingPort->parentItem())
            {
                break;  // 找到目标端口，退出循环
            }
        }
        if(targetPort)
        {
            if((targetPort->portType() == QBlueprintPort::EVENT_INPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_OUTPUT)
                ||(targetPort->portType() == QBlueprintPort::EVENT_OUTPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_INPUT)
                ||(targetPort->portType() == QBlueprintPort::EVENT_INPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_FALSE_RETURN)
                ||(targetPort->portType() == QBlueprintPort::EVENT_INPUT && m_currentConnection->startPort()->portType() == QBlueprintPort::EVENT_TRUE_RETURN))
            {
                qDebug() << "事件端口连接";
                m_currentConnection->setEndPort(targetPort);
                m_draggingPort->sendDataToConnectedPorts();
                propagateDataFromInitialNode(m_currentConnection->startPort());
            }
            else if (areTypesCompatible(m_currentConnection->startPort()->getVarTypeName(),targetPort->getVarTypeName())
                     && targetPort->portType()!=QBlueprintPort::EVENT_INPUT && targetPort->portType()!=QBlueprintPort::EVENT_OUTPUT)
            {
                qDebug() << "Found target port:" << targetPort->name();
                // 连接两个端口
                m_currentConnection->setEndPort(targetPort);
                m_draggingPort->sendDataToConnectedPorts();
            }
            else if(m_currentConnection->startPort()->getVarTypeName()==targetPort->getVarTypeName())
                qDebug() << "真的吗";
            else
            {
                qDebug() << m_currentConnection->startPort()->getVarTypeName() << " vs " << targetPort->getVarTypeName();
                removeConnection(m_currentConnection); // 删除连接
            }
        }
        else
            removeConnection(m_currentConnection);


        m_currentConnection = nullptr;
        m_draggingPort = nullptr;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void QBlueprint::updateConnectionsForPort(QBlueprintPort *port)
{
    // 遍历所有连接并更新与指定端口相关的连接
    for (QBlueprintConnection *connection : connections)
    {
        if (connection->startPort() == port)
        {
            connection->updatePosition(port->centerPos(), connection->endPort()->centerPos());
        }
        else if (connection->endPort() == port)
        {
            connection->updatePosition(connection->startPort()->centerPos(), port->centerPos());
        }
    }
}

void QBlueprint::updateAllConnections()
{
    for (QBlueprintConnection* connection : connections)
    {
        if (connection->startPort() && connection->endPort())
        {
            connection->updatePosition(connection->startPort()->centerPos(), connection->endPort()->centerPos());
        }
    }
}


// 添加连接到列表中
void QBlueprint::addConnection(QBlueprintConnection* connection)
{
    connections.push_back(connection);
    scene->addItem(connection); // 将连接添加到场景中
}

// 从列表中移除连接
void QBlueprint::removeConnection(QBlueprintConnection* connection)
{
    connections.erase(std::remove(connections.begin(), connections.end(), connection), connections.end());
    scene->removeItem(connection); // 从场景中移除连接
    delete connection;
}

// 在 startConnectionDrag 中使用 addConnection
void QBlueprint::startConnectionDrag(const QPointF &startPos)
{
    // 创建临时连线
    m_currentConnection = new QBlueprintConnection(m_draggingPort, nullptr);
    addConnection(m_currentConnection); // 将连接添加到管理列表中

    // 设置初始位置
    m_currentConnection->updatePosition(m_draggingPort->centerPos(), startPos);

    // 强制刷新场景
    scene->update();
}

void QBlueprint::createOutputNode()
{
    for (int i = 1; i < NUM_DATA_TYPES; ++i) {
        addOutputNode(static_cast<DataType>(i));
    }
}

void QBlueprint::createInputNode()
{
    for (int i = 1; i < NUM_DATA_TYPES; ++i) {
        addInputNode(static_cast<DataType>(i));
    }
}

void QBlueprint::createControlNode()
{
    QBlueprintNode* node_branch = new QBlueprintNode(Type::BRANCH);
    node_branch->setClassName("Control");
    node_branch->setNodeTitle("Branch");
    save_nodes.push_back(node_branch);

    QBlueprintNode* node_forloop = new QBlueprintNode(Type::FORLOOP);
    node_forloop->setClassName("Control");
    node_forloop->setNodeTitle("ForLoop");
    save_nodes.push_back(node_forloop);

    QBlueprintNode* node_condition = new QBlueprintNode(Type::CONDITION,DataType::VARIANT);
    node_condition->setClassName("Control");
    node_condition->setNodeTitle("Condition");
    save_nodes.push_back(node_condition);

    QBlueprintNode* node_begin = new QBlueprintNode(Type::BEGIN);
    node_begin->setClassName("Control");
    node_begin->setNodeTitle("Begin");
    save_nodes.push_back(node_begin);
}

void QBlueprint::addOutputNode(DataType dataType)
{
    QBlueprintNode* node = new QBlueprintNode(Type::OUTPUT,dataType);
    node->setClassName("Output");
    node->setNodeTitle(getEnumName(dataType));
    save_nodes.push_back(node);
}
void QBlueprint::addInputNode(DataType dataType)
{
    QBlueprintNode* node = new QBlueprintNode(Type::INPUT,dataType);
    node->setClassName("Input");
    node->setNodeTitle(getEnumName(dataType));
    save_nodes.push_back(node);
}
bool QBlueprint::isEventPortConnected(QBlueprintPort* outputPort, QBlueprintPort* inputPort) const {
    for (QBlueprintConnection* connection : connections) {
        qDebug() << "connection->startPort():" << connection->startPort()->portType() << "connection->endPort():" << connection->endPort()->portType();
        if ((/*connection->startPort() == outputPort && connection->endPort() == inputPort &&*/
             connection->startPort()->portType() == QBlueprintPort::EVENT_OUTPUT &&
             connection->endPort()->portType() == QBlueprintPort::EVENT_INPUT) ||
            (/*connection->startPort() == inputPort && connection->endPort() == outputPort &&*/
             connection->startPort()->portType() == QBlueprintPort::EVENT_INPUT &&
             connection->endPort()->portType() == QBlueprintPort::EVENT_OUTPUT)) {
            return true;
        }
        else if((connection->startPort()->data() == "true" && connection->startPort()->portType() == QBlueprintPort::EVENT_TRUE_RETURN
                   && connection->endPort()->portType() == QBlueprintPort::EVENT_INPUT)||
                   (connection->startPort()->data() == "true" && connection->startPort()->portType() == QBlueprintPort::EVENT_INPUT
                       && connection->endPort()->portType() == QBlueprintPort::EVENT_TRUE_RETURN)){
            return true;
            qDebug() << "true";
        }
        else if((connection->startPort()->data() == "false" && connection->startPort()->portType() == QBlueprintPort::EVENT_FALSE_RETURN
                    && connection->endPort()->portType() == QBlueprintPort::EVENT_INPUT)||
                   (connection->startPort()->data() == "false" && connection->startPort()->portType() == QBlueprintPort::EVENT_INPUT
                    && connection->endPort()->portType() == QBlueprintPort::EVENT_FALSE_RETURN)){
            return true;
            qDebug() << "false";
        }
    }
    return false;
}
void QBlueprint::handleForLoopCompletion(QBlueprintPort* completedPort) {
    // 找到对应的ForLoop节点
    QBlueprintNode* forLoopNode = dynamic_cast<QBlueprintNode*>(completedPort->parentItem());
    if (!forLoopNode) return;

    // 发送完成信号给ForLoop节点
    for (QBlueprintPort* port : forLoopNode->getInputPorts()) {
        if (port->name() == "LoopCompleted") {
            port->receiveData(QVariant::fromValue(true));
            break;
        }
    }
}
void QBlueprint::propagateDataFromInitialNode(QBlueprintPort* initialPort)
{
    if (!initialPort) return;
    if (initialPort->name() == "LoopCompleted") {
        handleForLoopCompletion(initialPort);
        return;
    }
    // 获取初始端口的父节点
    QBlueprintNode* initialNode = dynamic_cast<QBlueprintNode*>(initialPort->parentItem());
    if (!initialNode) return;

    // 找到该节点的所有输出端口，并发送数据
    for (QBlueprintPort* outputPort : initialNode->getOutputPorts())
    {
        outputPort->sendDataToConnectedPorts();
    }
}
bool QBlueprint::isNumericType(const QString& type)
{
    return (type == "int" || type == "float" || type == "double" ||
            type == "short" || type == "long" || type == "unsigned int");
}
// 类型兼容性检查函数
bool QBlueprint::areTypesCompatible(const QString& type1, const QString& type2)
{
    if (type1 == type2) return true;

    // 特殊处理：空类型或未知类型
    if (type1.isEmpty() || type2.isEmpty() || type1 == "QVariant" || type2 == "QVariant") {
        return true;
    }

    // 数值类型兼容性
    if (isNumericType(type1) && isNumericType(type2)) return true;

    // 字符串类型兼容性
    if ((type1.contains("String") || type1 == "char*") &&
        (type2.contains("String") || type2 == "char*")) {
        return true;
        }

    // QImage 相关类型兼容性
    if ((type1.contains("Image") || type1.contains("Pixmap")) &&
        (type2.contains("Image") || type2.contains("Pixmap"))) {
        return true;
        }

    return false;
}
void QBlueprint::testForLoop()
{
    // 找到ForLoop节点的事件输入端口
    for (QBlueprintNode* node : save_nodes) {
        if (node->getNodeType() == Type::FORLOOP) {
            for (QBlueprintPort* port : node->getInputPorts()) {
                if (port->portType() == QBlueprintPort::EVENT_INPUT) {
                    // 手动触发事件
                    port->receiveData(QVariant::fromValue(true));
                    qDebug() << "Manually triggered ForLoop";
                    break;
                }
            }
        }
    }
}



#pragma once
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <cmath>
#include <QMenu>
#include "qblueprintnode.h"
#include "qblueprintconnection.h"
#include "qnodefactory.h"

/****************************************************/
// 在这里添加你的头文件
#include <QWidgetAction>

#include "testclass.h"
#include "math.h"
#include "qts.h"
#ifdef OPENCV_FOUND
#include "opencv.h"
#endif
/****************************************************/
class QBlueprint : public QGraphicsView
{
    Q_OBJECT

public:
    explicit QBlueprint(QWidget *parent = nullptr);
    ~QBlueprint();
    void removeConnection(QBlueprintConnection* connection);

    // 更新与指定端口相关的所有连接
    void updateConnectionsForPort(QBlueprintPort *port);
    // 存储所有连接的列表
    std::vector<QBlueprintConnection*> connections;
    //void addBlueprintNode(QString funcname);
    void createBlueprintNodes();
    static int add(int a, int b, int c);
    static int deletea(int a);
    void placeNodeInScene(QBlueprintNode *originalNode, const QPointF& mousePos);
    void pushVectorQBlueprintNode(QBlueprintNode* node);
    bool isEventPortConnected(QBlueprintPort *outputPort, QBlueprintPort *inputPort) const;

    void handleForLoopCompletion(QBlueprintPort *completedPort);

    void propagateDataFromInitialNode(QBlueprintPort *initialPort);
    QGraphicsScene *getScene() { return scene; }
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;

    bool shouldShowNode(QBlueprintNode *node, const QString &searchText);

    void filterMenuItems();

    void onSearchTextChanged(const QString &text);

    void addNodeToMenu(QMenu *menu, QBlueprintNode *node, const QPointF &scenePos);

    bool eventFilter(QObject *obj, QEvent *event);

    void toggleFavorite(QBlueprintNode *node);

private:
    const double minScaleFactor = 0.1;
    const double maxScaleFactor = 8.0;
    QGraphicsScene *scene;

    std::vector<QBlueprintNode*> save_nodes;    // 用于存储所有节点
    std::vector<QBlueprintNode*> scene_nodes;   // 用于存储场景中的节点
    QList<QBlueprintNode*> m_favoriteNodes;     // 用于存储收藏节点

    QMenu* m_currentContextMenu = nullptr; // 当前打开的菜单
    QPoint m_currentEventPos;              // 当前菜单的位置
    QString m_searchText;                  // 当前搜索文本
    QLineEdit* m_searchEdit = nullptr;     // 搜索框指针
    bool m_isClickingStar = false;         // 标记是否正在点击星标
    bool m_isClickingSearch = false;       // 标记是否正在点击搜索框
    QMap<QMenu*, QList<QWidgetAction*>> m_menuActions; // 存储菜单项以便过滤
    // 添加和移除连接的方法
    void addConnection(QBlueprintConnection* connection);

    QBlueprintPort *m_draggingPort = nullptr;
    QBlueprintConnection *m_currentConnection = nullptr;
    void startConnectionDrag(const QPointF &startPos);
    void updateAllConnections();
    void classifyNodes();
    void createOutputNode(); // 输出节点,又连接输入,显示信息，同时也可再输出
    void createInputNode(); // 输入节点,用户输入,节点输出用户输入的内容
    void createControlNode(); // 控制节点,if-else,判断大小,for语句
    void addOutputNode(DataType dataType);
    void addInputNode(DataType dataType);
    bool isNumericType(const QString &type);
    bool areTypesCompatible(const QString &type1, const QString &type2);

    void testForLoop();
};


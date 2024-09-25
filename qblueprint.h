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
#include "testclass.h"
#include "math.h"
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
    void propagateDataFromInitialNode(QBlueprintPort *initialPort);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void contextMenuEvent(QContextMenuEvent *event) override;
private:
    const double minScaleFactor = 0.1;
    const double maxScaleFactor = 8.0;
    QGraphicsScene *scene;

    std::vector<QBlueprintNode*> save_nodes;    // 用于存储所有节点
    std::vector<QBlueprintNode*> scene_nodes;   // 用于存储场景中的节点


    // 添加和移除连接的方法
    void addConnection(QBlueprintConnection* connection);

    QBlueprintPort *m_draggingPort = nullptr;
    QBlueprintConnection *m_currentConnection = nullptr;
    void startConnectionDrag(const QPointF &startPos);
    void updateAllConnections();
    void classifyNodes();

    void createOutputNode(); // 输出节点,又连接输入,显示信息，同时也可再输出
    void createInputNode(); // 输入节点,用户输入,节点输出用户输入的内容

    void addOutputNode(DataType dataType);
    void addInputNode(DataType dataType);
    bool isNumericType(const QString &type);
    bool areTypesCompatible(const QString &type1, const QString &type2);
};


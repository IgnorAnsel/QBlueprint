#ifndef QBLUEPRINT_H
#define QBLUEPRINT_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include "qblueprintnode.h"
#include "qblueprintconnection.h"

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
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const double minScaleFactor = 0.1;
    const double maxScaleFactor = 8.0;
    QGraphicsScene *scene;



    // 添加和移除连接的方法
    void addConnection(QBlueprintConnection* connection);

    QBlueprintPort *m_draggingPort = nullptr;
    QBlueprintConnection *m_currentConnection = nullptr;
    void startConnectionDrag(const QPointF &startPos);
};

#endif // QBLUEPRINT_H

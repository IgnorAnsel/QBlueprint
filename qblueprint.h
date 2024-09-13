#ifndef QBLUEPRINT_H
#define QBLUEPRINT_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include "qblueprintnode.h"
class QBlueprint : public QGraphicsView
{
    Q_OBJECT

public:
    explicit QBlueprint(QWidget *parent = nullptr);
    ~QBlueprint();
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;  // 重载绘制背景方法
    void wheelEvent(QWheelEvent *event) override;  // 重载滚轮事件方法

private:
    const double minScaleFactor = 0.1;  // 最小缩放比例
    const double maxScaleFactor = 8.0; // 最大缩放比例
    QGraphicsScene *scene;  // 添加场景成员变量
};

#endif // QBLUEPRINT_H

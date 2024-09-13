#ifndef QBLUEPRINT_H
#define QBLUEPRINT_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
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
    QGraphicsScene *scene;  // 添加场景成员变量
};

#endif // QBLUEPRINT_H

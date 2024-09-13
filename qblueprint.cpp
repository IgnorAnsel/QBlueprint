#include "qblueprint.h"
#include "ui_qblueprint.h"

QBlueprint::QBlueprint(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this))  // 初始化场景
{
    // 设置场景的范围，可以根据需要调整
    scene->setSceneRect(-1000, -1000, 1000, 1000);

    // 将场景设置为QGraphicsView的场景
    setScene(scene);

    // 可选：设置视图的一些属性，比如抗锯齿等
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);  // 设置拖拽模式
    // 初始视图缩放
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setBackgroundBrush(QColor(30, 30, 30));  // 设置深色背景
}

QBlueprint::~QBlueprint()
{

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

// 重载滚轮事件方法，实现缩放
void QBlueprint::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;  // 缩放因子

    // 获取鼠标在视图中的位置
    QPointF viewPos = event->position();

    // 将视图坐标转换为场景坐标
    QPointF scenePos = mapToScene(viewPos.toPoint());

    // 根据滚轮滚动方向进行缩放
    if (event->angleDelta().y() > 0)  // 如果滚轮向上滚动
    {
        scale(scaleFactor, scaleFactor);  // 放大视图
    }
    else  // 如果滚轮向下滚动
    {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // 缩小视图
    }

    // 获取缩放后的鼠标位置在场景中的位置
    QPointF newScenePos = mapToScene(viewPos.toPoint());

    // 计算视图需要移动的距离，以使得缩放后鼠标位置不变
    QPointF delta = newScenePos - scenePos;

    // 调整视图的中心，使得缩放后的鼠标位置保持不变
    setTransformationAnchor(QGraphicsView::NoAnchor);
    translate(delta.x(), delta.y());

    event->accept();  // 接受事件
}

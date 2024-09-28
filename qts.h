#ifndef QTS_H
#define QTS_H
#include<qpoint.h>
#include <utility>
class Qts
{
public:
    Qts();
    static QPointF setQPointF(double x_pos, double y_pos);
    static double getQPointF_X(QPointF qpointf);
    static double getQPointF_Y(QPointF qpointf);

    static QPoint setQPoint(int x_pos, int y_pos);
    static int getQPoint_X(QPoint qpoint);
    static int getQPoint_Y(QPoint qpoint);

};

#endif // QTS_H

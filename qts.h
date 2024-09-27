#ifndef QTS_H
#define QTS_H
#include<qpoint.h>
#include <utility>
class Qts
{
public:
    Qts();
    static QPointF setQPointF(double x_pos, double y_pos);
    static std::pair<double, double> getQPointF(QPointF qpointf);
    static QPoint setQPoint(int x_pos, int y_pos);
    static std::pair<int, int> getQPoint(QPoint qpoint);
};

#endif // QTS_H

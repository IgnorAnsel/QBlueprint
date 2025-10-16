#ifndef QTS_H
#define QTS_H
#include <qpoint.h>
#include <QString>
#include <QStringList>
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
    static QStringList inputNames_setQPointF;
    static QString outputName_setQPointF;

    static QStringList inputNames_getQPointF_X;
    static QString outputName_getQPointF_X;

    static QStringList inputNames_getQPointF_Y;
    static QString outputName_getQPointF_Y;

    static QStringList inputNames_setQPoint;
    static QString outputName_setQPoint;

    static QStringList inputNames_getQPoint_X;
    static QString outputName_getQPoint_X;

    static QStringList inputNames_getQPoint_Y;
    static QString outputName_getQPoint_Y;
};

#endif // QTS_H

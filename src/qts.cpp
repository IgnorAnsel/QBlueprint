#include "qts.h"

Qts::Qts() {}

QPointF Qts::setQPointF(double x_pos, double y_pos)
{
    return QPointF(x_pos,y_pos);
}

double Qts::getQPointF_X(QPointF qpointf)
{
    return qpointf.x();
}

double Qts::getQPointF_Y(QPointF qpointf)
{
    return qpointf.y();
}

QPoint Qts::setQPoint(int x_pos, int y_pos)
{
    return QPoint(x_pos,y_pos);
}

int Qts::getQPoint_X(QPoint qpoint)
{
    return qpoint.x();
}

int Qts::getQPoint_Y(QPoint qpoint)
{
    return qpoint.y();
}
QStringList Qts::inputNames_setQPointF = {"x_pos", "y_pos"};
QString Qts::outputName_setQPointF = "QPointF";

QStringList Qts::inputNames_getQPointF_X = {"qpointf"};
QString Qts::outputName_getQPointF_X = "x";

QStringList Qts::inputNames_getQPointF_Y = {"qpointf"};
QString Qts::outputName_getQPointF_Y = "y";

QStringList Qts::inputNames_setQPoint = {"x_pos", "y_pos"};
QString Qts::outputName_setQPoint = "QPoint";

QStringList Qts::inputNames_getQPoint_X = {"qpoint"};
QString Qts::outputName_getQPoint_X = "x";

QStringList Qts::inputNames_getQPoint_Y = {"qpoint"};
QString Qts::outputName_getQPoint_Y = "y";

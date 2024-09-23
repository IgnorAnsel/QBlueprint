#ifndef ENTERLIMITER_H
#define ENTERLIMITER_H
#include "qblueprintport.h"
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
void setEnterLimiter(QLineEdit* lineEdit, QBlueprintPort* port);

class EnterLimiter
{
public:
    EnterLimiter();

};

#endif // ENTERLIMITER_H

#include "enterlimiter.h"

EnterLimiter::EnterLimiter() {}

void setEnterLimiter(QLineEdit *lineEdit, QBlueprintPort *port)
{
    QRegularExpression regExp;
    QRegularExpressionValidator* validator;

    switch (port->portDataType())
    {
    case DataType::INT:
    case DataType::LONG:
    case DataType::SHORT:
    case DataType::UNSIGNED_INT:
        // 只允许输入整数
        regExp.setPattern("^-?\\d+$");
        break;

    case DataType::FLOAT:
    case DataType::DOUBLE:
        // 允许输入浮点数（正负小数）
        regExp.setPattern("^-?\\d*\\.?\\d+$");
        break;

    case DataType::CHAR:
        // 只允许输入一个字符
        regExp.setPattern("^.$");
        break;

    case DataType::STRING:
    case DataType::QSTRING:
        // 允许输入任意字符串
        regExp.setPattern("^.*$");
        break;

    case DataType::BOOL:
        // 只允许输入 true 或 false（忽略大小写）
        regExp.setPattern("^(true|false|True|False)$");
        break;

    case DataType::QTIME:
        // 允许输入时间格式 (HH:MM:SS)
        regExp.setPattern("^\\d{2}:\\d{2}:\\d{2}$");
        break;

    case DataType::QPOINT:
    case DataType::QPOINTF:
    case DataType::QSIZE:
    case DataType::QSIZEF:
    case DataType::QRECT:
    case DataType::QRECTF:
        // 允许输入格式 (x, y) 或 (x, y, width, height)
        regExp.setPattern("^\\(\\d+, \\d+(, \\d+, \\d+)?\\)$");
        break;

    case DataType::QCOLOR:
        // 允许输入颜色十六进制 (#RRGGBB)
        regExp.setPattern("^#([A-Fa-f0-9]{6})$");
        break;

    case DataType::QPIXMAP:
    case DataType::QIMAGE:
        // 不做任何限制，可以根据需要定制
        regExp.setPattern("^.*$");
        break;

    default:
        // 默认不做限制
        regExp.setPattern("^.*$");
        break;
    }

    validator = new QRegularExpressionValidator(regExp, lineEdit);
    lineEdit->setValidator(validator);
}

#ifndef ALLUSE_H
#define ALLUSE_H
#include <QString>
#include <QVariant>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include <QPen>
#include <QBrush>
#include <QFont>
#define NUM_DATA_TYPES 22
enum Type{FUNCTION,INPUT,OUTPUT,BRANCH,CONDITION,FORLOOP};
enum DataType {
    FOR_FUNCTION,          // 为FUNCTION使用
    INT,           // 整型
    FLOAT,         // 单精度浮点型
    DOUBLE,        // 双精度浮点型
    CHAR,          // 字符型
    STRING,        // 字符串型
    BOOL,          // 布尔型
    LONG,          // 长整型
    SHORT,         // 短整型
    UNSIGNED_INT,  // 无符号整型
    VARIANT,       // QVariant 类型 (Qt通用类型)
    QSTRING,       // QString 类型 (Qt字符串类型)
    QTIME,         // QTime 类型 (Qt时间类型)
    QPOINT,        // QPoint 类型 (Qt坐标点类型)
    QPOINTF,       // QPointF 类型 (Qt浮点坐标点类型)
    QSIZE,         // QSize 类型 (Qt尺寸类型)
    QSIZEF,        // QSizeF 类型 (Qt浮点尺寸类型)
    QRECT,         // QRect 类型 (Qt矩形类型)
    QRECTF,        // QRectF 类型 (Qt浮点矩形类型)
    QCOLOR,        // QColor 类型 (Qt颜色类型)
    QPIXMAP,       // QPixmap 类型 (Qt图像类型)
    QIMAGE,        // QImage 类型 (Qt图像类型)
};

static QString DataTypeNames[] = {
    "FOR_FUNCTION",
    "INT",
    "FLOAT",
    "DOUBLE",
    "CHAR",
    "STRING",
    "BOOL",
    "LONG",
    "SHORT",
    "UNSIGNED_INT",
    "VARIANT",
    "QSTRING",
    "QTIME",
    "QPOINT",
    "QPOINTF",
    "QSIZE",
    "QSIZEF",
    "QRECT",
    "QRECTF",
    "QCOLOR",
    "QPIXMAP",
    "QIMAGE",
};
static QString getEnumName(DataType dataType) {
    if (dataType >= 0 && dataType < NUM_DATA_TYPES) {
        return DataTypeNames[dataType];
    }
    return "UNKNOWN";
}
// 通过字符串获取对应的枚举值
static DataType getEnumFromName(const QString& name) {
    for (int i = 0; i < NUM_DATA_TYPES; ++i) {
        if (DataTypeNames[i].compare(name, Qt::CaseInsensitive) == 0) {
            return static_cast<DataType>(i); // 返回对应的枚举值
        }
    }
    return FOR_FUNCTION; // 如果未找到，返回无效的值
}
class AllUse
{
public:
    AllUse();
};

#endif // ALLUSE_H

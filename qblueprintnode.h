#ifndef QBLUEPRINTNODE_H
#define QBLUEPRINTNODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <vector>
#include <algorithm>
#include <QLineEdit>
#include <QLabel>
#include "imagelabel.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QObject>
#include "alluse.h"
#include "qblueprintport.h"
#include "qblueprintconnection.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>


#include "testclass.h"
class QBlueprint;
class QBlueprintNode : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    QBlueprintNode(enum Type Type,DataType datatype = DataType::FOR_FUNCTION, QGraphicsItem *parent = nullptr);
    ~QBlueprintNode();
    // 重载boundingRect()方法，定义绘制区域
    QRectF boundingRect() const override;

    // 重载paint()方法，定义绘制节点的具体方式
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 添加输入和输出端口
    QBlueprintPort* addInputPort();
    QBlueprintPort* addOutputPort();
    // 添加输入和输出端口（带名称）
    QBlueprintPort* addInputPort(const QString &name);
    QBlueprintPort* addOutputPort(const QString &name);
    // 添加输入和输出端口（为事件的）
    QBlueprintPort* addInputPort(enum Type Type);
    QBlueprintPort* addOutputPort(enum Type Type);
    const std::vector<QBlueprintPort *>& getInputPorts() const { return inputPorts; }
    const std::vector<QBlueprintPort *>& getOutputPorts() const { return outputPorts; }

    void setNodeTitle(QString name);
    void setClassName(QString class_name);
    // 获取类名
    QString getClassName() const { return class_name; }
    // 获取节点名称
    QString getNodeTitle() const { return m_name; }
    enum Type getNodeType() { return nodeType; }
    enum Type setNodeType(enum Type type){ nodeType = type; }
    QBlueprintNode *clone() const;

    // 数据类型管理方法
    void addDataType(DataType type) { dataTypes.push_back(type); }
    const std::vector<DataType>& getDataTypes() const { return dataTypes; }
    void updateLabelWithData(QBlueprintPort *port, const QString &data);
    void processData(QBlueprintPort *inputPort, const QVariant &data);
    bool isPortConnected(QBlueprintPort *inputPort, QBlueprintPort *outputPort);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void setQVariantType(QBlueprintPort* port);
    void customNodePortSort();
    void imageNodePortSort();
    void initInputOrOutput(enum Type Type, DataType datatype);
    void initData(DataType datatype);
    void cleanData();
    QFont font;
    enum Type nodeType;
    DataType dataType;
    QString class_name;
    QString m_name;
    std::vector<QBlueprintPort *> inputPorts;   // 存储输入端口
    std::vector<QBlueprintPort *> outputPorts;  // 存储输出端口
    std::vector<DataType> dataTypes;            // 存储节点支持的数据类型
    //------------------------- 节点数据 --------------------------------//
    std::vector<int>* intData = nullptr;               // INT
    std::vector<float>* floatData = nullptr;           // FLOAT
    std::vector<double>* doubleData = nullptr;         // DOUBLE
    std::vector<char>* charData = nullptr;             // CHAR
    std::vector<std::string>* stringData = nullptr;    // STRING
    std::vector<bool>* boolData = nullptr;             // BOOL
    std::vector<long>* longData = nullptr;             // LONG
    std::vector<short>* shortData = nullptr;           // SHORT
    std::vector<unsigned int>* unsignedIntData = nullptr;  // UNSIGNED_INT
    std::vector<QVariant>* variantData = nullptr;      // VARIANT
    std::vector<QString>* qStringData = nullptr;       // QSTRING
    std::vector<QPoint>* qPointData = nullptr;         // QPOINT
    std::vector<QPointF>* qPointFData = nullptr;       // QPOINTF
    std::vector<QSize>* qSizeData = nullptr;           // QSIZE
    std::vector<QSizeF>* qSizeFData = nullptr;         // QSIZEF
    std::vector<QRect>* qRectData = nullptr;           // QRECT
    std::vector<QRectF>* qRectFData = nullptr;         // QRECTF
    std::vector<QColor>* qColorData = nullptr;         // QCOLOR
    std::vector<QPixmap>* qPixmapData = nullptr;       // QPIXMAP
    std::vector<QImage>* qImageData = nullptr;         // QIMAGE
    //------------------------- 节点输入 --------------------------------//
    std::vector<QLineEdit*> lineEdits;
    std::vector<QLabel*> inputlabel;
    std::vector<QLabel*> outputlabel;
    std::vector<ImageLabel*> inputImagelabel;
    std::vector<ImageLabel*> outputImagelabel;
    void addLineEdit(QBlueprintPort* port);
    void addInputLabel(QBlueprintPort* port);
    void addOutputLabel(QBlueprintPort* outport, QBlueprintPort* inport);
    void addButtonToTopLeft();
    void adjustLineEditWidth(const QString &text);
    void adjustLabelWidth(const QString &text);


    //
    TestClass testClassInstance;

    QVariant mathFunctions();
    QVariant opencvFunctions();

};

#endif // QBLUEPRINTNODE_H

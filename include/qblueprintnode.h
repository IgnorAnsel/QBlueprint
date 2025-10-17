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
#include <QComboBox>
#include <QRadioButton>

#include "testclass.h"
class QBlueprint;
class QBlueprintNode : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:

    QBlueprintNode(enum Type Type,DataType datatype = DataType::FOR_FUNCTION, QGraphicsItem *parent = nullptr);

    void setupForLoopPorts();

    ~QBlueprintNode();
    // 重载boundingRect()方法，定义绘制区域
    QRectF boundingRect() const override;

    // 重载paint()方法，定义绘制节点的具体方式
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void addInputPortCondition(enum Type Type);
    // 添加输入和输出端口
    QBlueprintPort* addInputPort();
    QBlueprintPort* addOutputPort();
    // 添加输入和输出端口（带名称）
    QBlueprintPort* addInputPort(const QString &name);
    QBlueprintPort* addOutputPort(const QString &name);
    // 添加输入和输出端口（为事件的）
    void addInputPort(enum Type Type);
    void addOutputPort(enum Type Type);
    // 有端口的信息简介时
    QBlueprintPort* addInputPort(const QString &name, const QString &brief);
    QBlueprintPort* addOutputPort(const QString &name, const QString &brief);
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

    QVariant getPortValue(const QString &portName);

    void setPortValue(const QString &portName, const QVariant &value);

    void processData(QBlueprintPort *inputPort, const QVariant &data);
    bool isPortConnected(QBlueprintPort *inputPort, QBlueprintPort *outputPort);
    void addRadioButtonOptions(QBlueprintPort *port);

    void processForLoopData(QBlueprintPort *inputPort, const QVariant &data);

    void processBranchData(QBlueprintPort *inputPort, const QVariant &data);

    void executeForLoopIteration(int index, int end, int step);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void setQVariantType(QBlueprintPort* port);
    void customNodePortSort();
    void imageNodePortSort();
    QFont font;
    enum Type nodeType;
    DataType dataType;
    QString class_name;
    QString m_name;
    std::vector<QBlueprintPort *> inputPorts;   // 存储输入端口
    std::vector<QBlueprintPort *> outputPorts;  // 存储输出端口
    std::vector<DataType> dataTypes;            // 存储节点支持的数据类型

    //------------------------- 节点输入 --------------------------------//
    std::vector<QLineEdit*> lineEdits;
    std::vector<QLabel*> inputlabel;
    std::vector<QLabel*> outputlabel;
    std::vector<QComboBox*> comboboxs;
    std::vector<ImageLabel*> inputImagelabel;
    std::vector<ImageLabel*> outputImagelabel;
    std::vector<std::pair<QRadioButton*, QRadioButton*>> radioButtonOptions; // 存放关系 || &&
    std::vector<QString> radioButtonValues;
    std::vector<QString> relation; // 存放关系 ><=等
    std::vector<bool> partbool; // 每一个部分的bool值
    void addLineEdit(QBlueprintPort* port);
    void addLineEdit(QBlueprintPort *port1, QBlueprintPort *port2);
    void addInputLabel(QBlueprintPort* port);
    void addLabelToPort(QBlueprintPort *port, const QString &text);
    void addOutputLabel(QBlueprintPort* outport, QBlueprintPort* inport);
    void addButtonToTopLeft(enum Type type);
    void addButtonToTopLeft();
    void adjustLineEditWidth(const QString &text);
    void adjustLabelWidth(const QString &text);


    //
    TestClass testClassInstance;

    QVariant mathFunctions();
    QVariant opencvFunctions();
    QVariant qtsFunctions();

};

#endif // QBLUEPRINTNODE_H

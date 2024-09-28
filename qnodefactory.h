#pragma once // 只允许包含一次

#include "qblueprintnode.h"
#include "qblueprint.h"
#include <type_traits>
#include <tuple>
#include <QRegularExpression>
class QBlueprint;
class QBlueprintNode;
// 一个用于将函数签名自动生成节点的工厂类
class QNodeFactory
{
public:
    // 模板函数，传递一个函数并生成对应的节点
 /**
 * @brief 创建一个基于函数的 QBlueprintNode 节点。
 *
 * 该函数接受一个函数指针（或可调用对象），并生成一个对应的 `QBlueprintNode` 节点。此节点将函数的输入参数类型解析为输入端口，并将输出参数类型解析为输出端口。
 *
 * @tparam Func 类型化模板参数，表示传入的函数类型或可调用对象的类型。
 * @param blueprint 指向 `QBlueprint` 的指针，用于管理节点的父蓝图。
 * @param func 传入的函数指针或可调用对象，其输入输出参数将用于创建节点的输入输出端口。
 * @param nodeName （可选）节点的名称，如果未指定，则默认使用函数名。如果函数名无法自动解析，用户需要手动填写。
 * @param className （可选）节点所属的类名，默认为 "Other" 类别，可以用于分类节点。
 * @return QBlueprintNode* 返回一个新的 `QBlueprintNode` 节点，该节点封装了传入的函数，并可用于蓝图执行。
 *
 * @note 该函数通过 `__PRETTY_FUNCTION__` 提取函数名以便自动生成节点名称，但如果自动提取不正常，请手动传入 `nodeName` 参数。
 *
 * @details 函数的具体步骤：
 * 1. 创建一个新的 `QBlueprintNode`，并将节点类型设置为 `Type::FUNCTION`。
 * 2. 如果没有传入 `nodeName`，则尝试从函数签名中提取函数名并设置为节点名称。
 * 3. 使用 `parseInputPorts()` 函数解析传入函数的输入参数类型，创建对应的输入端口。
 * 4. 使用 `parseOutputPort()` 函数解析传入函数的输出类型，创建对应的输出端口。
 * 5. 将节点加入到蓝图管理的节点列表中，并返回该节点。
 */
    template <typename Func>
    static QBlueprintNode* createNodeFromFunction(QBlueprint* blueprint, Func func, const QString& nodeName = QString(), const QString& className = "Other") // 自动获取函数名不正常，直接填写你需要的名称
    {
        QBlueprintNode* node = new QBlueprintNode(Type::FUNCTION);
        node->setClassName(className);
        // 如果没有提供节点名称，使用函数的类型名称
        if (nodeName.isEmpty()) {
            QString prettyFunction = __PRETTY_FUNCTION__; // 使用 __PRETTY_FUNCTION__
            QString funcName = extractFunctionName(prettyFunction);

            node->setNodeTitle(funcName);
        } else {
            node->setNodeTitle(nodeName);
        }
        node->setNodeType(Type::FUNCTION);
        // 解析输入参数类型
        parseInputPorts(node, func);

        // 解析输出参数类型
        parseOutputPort(node, func);
        blueprint->pushVectorQBlueprintNode(node);
        return node;
    }
    /**
 * @brief 创建一个基于函数的 QBlueprintNode 节点。
 *
 * 该函数接受一个函数指针（或可调用对象），并生成一个对应的 `QBlueprintNode` 节点。此节点将函数的输入参数类型解析为输入端口，并将输出参数类型解析为输出端口。
 *
 * @tparam Func 类型化模板参数，表示传入的函数类型或可调用对象的类型。
 * @param blueprint 指向 `QBlueprint` 的指针，用于管理节点的父蓝图。
 * @param func 传入的函数指针或可调用对象，其输入输出参数将用于创建节点的输入输出端口。
 * @param inputNames 输入参数的名称列表，包含所有输入参数的名称。
 * @param outputName 输出参数的名称，指定返回值的名称。
 * @param nodeName （可选）节点的名称，如果未指定，则默认使用函数名。如果函数名无法自动解析，用户需要手动填写。
 * @param className （可选）节点所属的类名，默认为 "Other" 类别，可以用于分类节点。
 * @return QBlueprintNode* 返回一个新的 `QBlueprintNode` 节点，该节点封装了传入的函数，并可用于蓝图执行。
 *
 * @note 该函数通过 `__PRETTY_FUNCTION__` 提取函数名以便自动生成节点名称，但如果自动提取不正常，请手动传入 `nodeName` 参数。
 *
 * @details 函数的具体步骤：
 * 1. 创建一个新的 `QBlueprintNode`，并将节点类型设置为 `Type::FUNCTION`。
 * 2. 如果没有传入 `nodeName`，则尝试从函数签名中提取函数名并设置为节点名称。
 * 3. 使用 `parseInputPorts()` 函数解析传入函数的输入参数类型和名称，创建对应的输入端口。
 * 4. 使用 `parseOutputPort()` 函数解析传入函数的输出类型和名称，创建对应的输出端口。
 * 5. 将节点加入到蓝图管理的节点列表中，并返回该节点。
 */
    template <typename Func>
    static QBlueprintNode* createNodeFromFunction(QBlueprint* blueprint, Func func, const QStringList& inputNames, const QString& outputName = QString(), const QString& nodeName = QString(), const QString& className = "Other")
    {
        QBlueprintNode* node = new QBlueprintNode(Type::FUNCTION);
        node->setClassName(className);

        if (nodeName.isEmpty()) {
            QString prettyFunction = __PRETTY_FUNCTION__;
            QString funcName = extractFunctionName(prettyFunction);
            node->setNodeTitle(funcName);
        } else {
            node->setNodeTitle(nodeName);
        }

        node->setNodeType(Type::FUNCTION);

        // 解析输入参数类型和名称
        parseInputPorts(node, func, inputNames);

        // 解析输出参数类型和名称
        parseOutputPort(node, func, outputName);

        blueprint->pushVectorQBlueprintNode(node);
        return node;
    }
private:
    static QString extractFunctionName(const QString& prettyFunction)
    {
        // 使用正则表达式从输入字符串中匹配函数名称
        QRegularExpression regex(R"((\w+)::(\w+))"); // 匹配 "ClassName::FunctionName" 这样的格式
        QRegularExpressionMatch match = regex.match(prettyFunction);

        if (match.hasMatch()) {
            // 提取函数名称部分
            QString functionName = match.captured(2);
            return functionName;
        }

        // 如果没有匹配到合适的部分，返回原始输入
        return prettyFunction;
    }

    // 处理普通函数指针
    template <typename Ret, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(*)(Args...), const QStringList& inputNames)
    {
        size_t index = 0;
        (..., (node->addInputPort(getTypeName<Args>(), inputNames[index++]))); // 使用参数名称
    }

    // 处理成员函数指针
    template <typename Ret, typename ClassType, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(ClassType::*)(Args...), const QStringList& inputNames)
    {
        size_t index = 0;
        (..., (node->addInputPort(getTypeName<Args>(), inputNames[index++]))); // 使用参数名称
    }

    // 解析输出类型 - 普通函数指针
    template <typename Ret, typename... Args>
    static void parseOutputPort(QBlueprintNode* node, Ret(*)(Args...), const QString& outputName)
    {
        node->addOutputPort(getTypeName<Ret>(), outputName);
    }

    // 解析输出类型 - 成员函数指针
    template <typename Ret, typename ClassType, typename... Args>
    static void parseOutputPort(QBlueprintNode* node, Ret(ClassType::*)(Args...), const QString& outputName)
    {
        node->addOutputPort(getTypeName<Ret>(), outputName);
    }





    // 处理普通函数指针
    template <typename Ret, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(*)(Args...))
    {
        (node->addInputPort(getTypeName<Args>()), ...);
    }

    // 处理成员函数指针
    template <typename Ret, typename ClassType, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(ClassType::*)(Args...))
    {
        qDebug() << "function";
        (node->addInputPort(getTypeName<Args>()), ...);
    }

    // 解析输出类型 - 普通函数指针
    template <typename Ret, typename... Args>
    static void parseOutputPort(QBlueprintNode* node, Ret(*)(Args...))
    {
        node->addOutputPort(getTypeName<Ret>());
    }

    // 解析输出类型 - 成员函数指针
    template <typename Ret, typename ClassType, typename... Args>
    static void parseOutputPort(QBlueprintNode* node, Ret(ClassType::*)(Args...))
    {
        node->addOutputPort(getTypeName<Ret>());
    }

    // 获取类型名称的辅助函数（简单示例，可以扩展以支持更多类型）
    template <typename T>
    static QString getTypeName()
    {
        if (std::is_same<T, int>::value)
            return "int";
        else if (std::is_same<T, float>::value)
            return "float";
        else if (std::is_same<T, double>::value)
            return "double";
        else if (std::is_same<T, QString>::value)
            return "QString";
        else if (std::is_same<T, QImage>::value)  // 添加对 QImage 的支持
            return "QImage";
        else if (std::is_same<T, QPoint>::value)
            return "QPoint";
        else if (std::is_same<T, QPointF>::value)
            return "QPointF";
        else
            return "unknown";
    }
};



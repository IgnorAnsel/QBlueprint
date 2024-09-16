#ifndef QNODEFACTORY_H
#define QNODEFACTORY_H

#include "qblueprintnode.h"
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
    template <typename Func>
    static QBlueprintNode* createNodeFromFunction(QBlueprint* blueprint, Func func, const QString& nodeName = QString(), const QString& className = "Other") // 自动获取函数名不正常，直接填写你需要的名称
    {
        QBlueprintNode* node = new QBlueprintNode();
        node->setClassName(className);
        // 如果没有提供节点名称，使用函数的类型名称
        if (nodeName.isEmpty()) {
            QString prettyFunction = __PRETTY_FUNCTION__; // 使用 __PRETTY_FUNCTION__
            QString funcName = extractFunctionName(prettyFunction);

            node->setNodeTitle(funcName);
        } else {
            node->setNodeTitle(nodeName);
        }
        // 解析输入参数类型
        parseInputPorts(node, func);

        // 解析输出参数类型
        parseOutputPort(node, func);
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
    static void parseInputPorts(QBlueprintNode* node, Ret(*)(Args...))
    {
        (node->addInputPort(getTypeName<Args>()), ...);
    }

    // 处理成员函数指针
    template <typename Ret, typename ClassType, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(ClassType::*)(Args...))
    {
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
        // 这里可以根据需要扩展更多的类型
        else
            return "unknown";
    }
};


#endif // QNODEFACTORY_H

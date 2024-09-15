#ifndef QNODEFACTORY_H
#define QNODEFACTORY_H

#include "qblueprintnode.h"
#include <type_traits>
#include <tuple>
class QBlueprintNode;
// 一个用于将函数签名自动生成节点的工厂类
class QNodeFactory
{
public:
    // 模板函数，传递一个函数并生成对应的节点
    template <typename Func>
    static QBlueprintNode* createNodeFromFunction(Func func, const QString& nodeName = "FunctionNode")
    {
        QBlueprintNode* node = new QBlueprintNode();

        // 设置节点名称（可选）
        node->setNodeTitle(nodeName);

        // 解析输入参数类型
        parseInputPorts(node, func);

        // 解析输出参数类型
        parseOutputPort(node, func);

        return node;
    }

private:
    // 解析输入参数（通过模板参数推断函数的参数类型）
    template <typename Ret, typename... Args>
    static void parseInputPorts(QBlueprintNode* node, Ret(*)(Args...))
    {
        // 对于每个参数类型，创建一个输入端口
        (node->addInputPort(getTypeName<Args>()), ...);
    }

    // 解析输出类型
    template <typename Ret, typename... Args>
    static void parseOutputPort(QBlueprintNode* node, Ret(*)(Args...))
    {
        // 为返回类型创建一个输出端口
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

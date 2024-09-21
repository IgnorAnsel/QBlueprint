// #include "qnodefactory.h"
// #include "qblueprint.h"

// template <typename Func>
// QBlueprintNode* QNodeFactory::createNodeFromFunction(QBlueprint* blueprint, Func func, const QString& nodeName, const QString& className)
// {
//     QBlueprintNode* node = new QBlueprintNode(Type::FUNCTION);
//     node->setClassName(className);
//     if (nodeName.isEmpty()) {
//         QString prettyFunction = __PRETTY_FUNCTION__;
//         QString funcName = extractFunctionName(prettyFunction);
//         node->setNodeTitle(funcName);
//     } else {
//         node->setNodeTitle(nodeName);
//     }
//     node->setNodeType(Type::FUNCTION);
//     parseInputPorts(node, func);
//     parseOutputPort(node, func);
//     blueprint->pushVectorQBlueprintNode(node); // 使用 QBlueprint 的成员
//     return node;
// }

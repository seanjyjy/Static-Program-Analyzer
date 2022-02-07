#include "UsesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

UsesExtractor::UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void UsesExtractor::mapUses(TNode *node, unordered_set<string> &usesSet) {
    if (usesSet.empty()) return;

    if (node->getType() == TNodeType::procedure) {
        procUsesMap.insert({node->getVal()->getVal(), usesSet});
    } else {
        stmtUsesMap.insert({nodeToStmtNumMap[node], usesSet});
    }
}

void UsesExtractor::dfs(TNode *node, unordered_set<string> &usesSet) {
    unordered_set<string> usesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], usesSetChild); // only 1 child stmtLst
        usesSet = usesSetChild;
        mapUses(node, usesSet);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSets(usesSet, usesSetChild);
            usesSetChild.clear();
        }
    } else if (type == TNodeType::printStmt) {
        usesSet = {node->getChildren()[0]->getVal()->getVal()}; // only 1 child varName
        mapUses(node, usesSet);
    } else if (type == TNodeType::whileStmt || type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSets(usesSet, usesSetChild);
            usesSetChild.clear();
        }
        mapUses(node, usesSet);
    } else if (type == TNodeType::assignStmt) {
        dfs(node->getChildren()[1], usesSetChild); // right child is expr
        usesSet = usesSetChild;
        mapUses(node, usesSet);
    } else if (isCondExpr(type) || isOp(type)) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSets(usesSet, usesSetChild);
            usesSetChild.clear();
        }
    } else if (type == TNodeType::varName) {
        usesSet = {node->getVal()->getVal()};
    }
}

void UsesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        unordered_set<string> st;
        dfs(procNode, st);
    }
}

unordered_map<string, unordered_set<string>> UsesExtractor::getProcUsesMap() {
    return procUsesMap;
}

unordered_map<string, unordered_set<string>> UsesExtractor::getStmtUsesMap() {
    return stmtUsesMap;
}

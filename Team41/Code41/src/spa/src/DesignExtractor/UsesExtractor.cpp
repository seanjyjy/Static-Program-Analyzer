#include "UsesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

UsesExtractor::UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                             unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        VarRelationExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder) {}

void UsesExtractor::dfs(TNode *node, unordered_set<string> &usesSet) {
    unordered_set<string> usesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], usesSet); // only 1 child stmtLst
        mapRelation(node, usesSet);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child: ch) {
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (type == TNodeType::printStmt) {
        usesSet = {node->getChildren()[0]->getTokenVal()}; // only 1 child varName
        mapRelation(node, usesSet);
    } else if (type == TNodeType::whileStmt || type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child: ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
        mapRelation(node, usesSet);
    } else if (type == TNodeType::assignStmt) {
        dfs(node->getChildren()[1], usesSetChild); // right child is expr
        usesSet = usesSetChild;
        mapRelation(node, usesSet);
    } else if (isCondExpr(type) || isOp(type)) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child: ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (type == TNodeType::varName) {
        usesSet = {node->getTokenVal()};
    }
}

void UsesExtractor::extract() {
    VarRelationExtractor::extract();
}

unordered_map<string, unordered_set<string>> UsesExtractor::getProcUsesMap() {
    return this->procRelationMap;
}

unordered_map<string, unordered_set<string>> UsesExtractor::getStmtUsesMap() {
    return this->stmtRelationMap;
}

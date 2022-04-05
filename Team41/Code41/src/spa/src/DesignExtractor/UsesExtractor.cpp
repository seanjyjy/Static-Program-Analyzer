#include "UsesExtractor.h"
#include "DesignExtractorUtils.h"

UsesExtractor::UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                             unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        VarRelationExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder) {}

void UsesExtractor::dfs(TNode *node, unordered_set<string> &usesSet) {
    unordered_set<string> usesSetChild;
    if (node->isProcedure()) {
        dfs(node->getChildren()[0], usesSet); // only 1 child stmtLst
        mapRelation(node, usesSet);
    } else if (node->isStmtLst()) {
        for (TNode *child: node->getChildren()) {
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (node->isPrint()) {
        usesSet = {node->getChildren()[0]->getTokenVal()}; // only 1 child varName
        mapRelation(node, usesSet);
    } else if (node->isWhile() || node->isIf()) {
        for (TNode *child: node->getChildren()) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
        mapRelation(node, usesSet);
    } else if (node->isAssign()) {
        dfs(node->getChildren()[1], usesSetChild); // right child is expr
        usesSet = usesSetChild;
        mapRelation(node, usesSet);
    } else if (node->isCondExpr() || node->isArithmeticOp()) {
        for (TNode *child: node->getChildren()) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (node->isVarName()) {
        usesSet = {node->getTokenVal()};
    }
}

bool UsesExtractor::extract() {
    return VarRelationExtractor::extract();
}

unordered_map<string, unordered_set<string>> UsesExtractor::getProcUsesMap() {
    return this->procRelationMap;
}

unordered_map<string, unordered_set<string>> UsesExtractor::getStmtUsesMap() {
    return this->stmtRelationMap;
}

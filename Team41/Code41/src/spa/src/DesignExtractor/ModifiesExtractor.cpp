#include "ModifiesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

ModifiesExtractor::ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                                     unordered_map<string, unordered_set<string>> &callsMap,
                                     list<string> &procCallOrder) :
        VarRelationExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder) {}

void ModifiesExtractor::dfs(TNode *node, unordered_set<string> &modifiesSet) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], modifiesSet); // only 1 child stmtLst
        mapRelation(node, modifiesSet);
    } else if (type == TNodeType::stmtLst) {
        unordered_set<string> modifiesSetChild;
        for (TNode *child: node->getChildren()) {
            dfs(child, modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
    } else if (type == TNodeType::readStmt || type == TNodeType::assignStmt) {
        modifiesSet = {node->getChildren()[0]->getTokenVal()}; // left child varName
        mapRelation(node, modifiesSet);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], modifiesSet); // right child stmtLst
        mapRelation(node, modifiesSet);
    } else if (type == TNodeType::ifStmt) {
        unordered_set<string> modifiesSetChild;
        const vector<TNode *> &ch = node->getChildren();
        for (size_t i = 1; i <= 2; ++i) { // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
        mapRelation(node, modifiesSet);
    }
}

void ModifiesExtractor::extract() {
    VarRelationExtractor::extract();
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getProcModifiesMap() {
    return this->procRelationMap;
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getStmtModifiesMap() {
    return this->stmtRelationMap;
}

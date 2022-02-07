#include "ModifiesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

ModifiesExtractor::ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void ModifiesExtractor::mapModifies(TNode *node, unordered_set<string> &modifiesSet) {
    if (modifiesSet.empty()) return;

    if (node->getType() == TNodeType::procedure)
        procModifiesMap.insert({node->getVal()->getVal(), modifiesSet});
    else
        stmtModifiesMap.insert({nodeToStmtNumMap[node], modifiesSet});
}

void ModifiesExtractor::dfs(TNode *node, unordered_set<string> &modifiesSet) {
    unordered_set<string> modifiesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], modifiesSetChild); // only 1 child stmtLst
        modifiesSet = modifiesSetChild;
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, modifiesSetChild);
            DesignExtractorUtils::combineSets(modifiesSet, modifiesSetChild);
            modifiesSetChild.clear();
        }
    } else if (type == TNodeType::readStmt || type == TNodeType::assignStmt) {
        modifiesSet = {node->getChildren()[0]->getVal()->getVal()}; // left child varName
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], modifiesSetChild); // right child stmtLst
        modifiesSet = modifiesSetChild;
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], modifiesSetChild);
            DesignExtractorUtils::combineSets(modifiesSet, modifiesSetChild);
            modifiesSetChild.clear();
        }
        mapModifies(node, modifiesSet);
    }
}

void ModifiesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        unordered_set<string> st;
        dfs(procNode, st);
    }
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getProcModifiesMap() {
    return procModifiesMap;
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getStmtModifiesMap() {
    return stmtModifiesMap;
}

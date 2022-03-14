#include "ParentExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

ParentExtractor::ParentExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void ParentExtractor::mapParent(TNode *node) {
    list<string> parentLst;
    TNodeType type = node->getType();
    if (type == TNodeType::ifStmt) {
        for (size_t i = 1; i <= 2; i++) {
            vector<TNode *> ch = node->getChildren()[i]->getChildren();
            for (TNode *child : ch)
                parentLst.push_back(nodeToStmtNumMap[child]);
        }
    } else { // TNodeType::whileStmt
        vector<TNode *> ch = node->getChildren()[1]->getChildren();
        for (TNode *child : ch)
            parentLst.push_back(nodeToStmtNumMap[child]);
    }
    parentMap.insert({nodeToStmtNumMap[node], parentLst});
}

void ParentExtractor::mapParentT(TNode *node, list<string> &parentLst) {
    if (parentLst.empty()) return;
    parentTMap.insert({nodeToStmtNumMap[node], parentLst});
}

void ParentExtractor::dfs(TNode *node, list<string> &parentLst) {
    list<string> parentLstChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], parentLstChild); // only 1 child stmtLst
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, parentLstChild);
            DesignExtractorUtils::combineListsClear(parentLst, parentLstChild);
        }
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // ifStmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], parentLstChild);
            DesignExtractorUtils::combineListsClear(parentLst, parentLstChild);;
        }
        mapParent(node);
        mapParentT(node, parentLst);
        parentLst.push_front(nodeToStmtNumMap[node]);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], parentLstChild); // right child stmtLst
        parentLst = parentLstChild;
        mapParent(node);
        mapParentT(node, parentLst);
        parentLst.push_front(nodeToStmtNumMap[node]);
    } else if (isStatement(type)) { // read, print, call, assign
        parentLst = {nodeToStmtNumMap[node]};
    }
}

void ParentExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        list<string> lst;
        dfs(procNode, lst);
    }
}

unordered_map<string, list<string>> ParentExtractor::getParentMap() {
    return this->parentMap;
}

unordered_map<string, list<string>> ParentExtractor::getParentTMap() {
    return this->parentTMap;
}

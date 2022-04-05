#include "ParentExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/AstNode/TNodeType.h"

ParentExtractor::ParentExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        StmtNumExtractor(ast, nodeToStmtNumMap) {}

void ParentExtractor::mapParent(TNode *node) {
    list<string> parentLst;
    TNodeType type = node->getType();
    if (type == TNodeType::ifStmt) {
        for (size_t i = 1; i <= 2; i++) {
            const vector<TNode *> &ch = node->getChildren()[i]->getChildren();
            for (TNode *child: ch)
                parentLst.push_back(nodeToStmtNumMap.at(child));
        }
    } else { // TNodeType::whileStmt
        const vector<TNode *> &ch = node->getChildren()[1]->getChildren();
        for (TNode *child: ch)
            parentLst.push_back(nodeToStmtNumMap.at(child));
    }
    parentMap.insert({nodeToStmtNumMap.at(node), parentLst});
}

void ParentExtractor::mapParentT(TNode *node, list<string> &parentLst) {
    if (parentLst.empty()) return;
    parentTMap.insert({nodeToStmtNumMap.at(node), parentLst});
}

void ParentExtractor::dfs(TNode *node, list<string> &parentLst) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], parentLst); // only 1 child stmtLst
    } else if (type == TNodeType::stmtLst) {
        list<string> parentLstChild;
        for (TNode *child: node->getChildren()) {
            dfs(child, parentLstChild);
            DesignExtractorUtils::combineListsClear(parentLst, parentLstChild);
        }
    } else if (type == TNodeType::ifStmt) {
        list<string> parentLstChild;
        const vector<TNode *> &ch = node->getChildren();
        for (size_t i = 1; i <= 2; ++i) { // ifStmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], parentLstChild);
            DesignExtractorUtils::combineListsClear(parentLst, parentLstChild);
        }
        mapParent(node);
        mapParentT(node, parentLst);
        parentLst.push_front(nodeToStmtNumMap.at(node));
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], parentLst); // right child stmtLst
        mapParent(node);
        mapParentT(node, parentLst);
        parentLst.push_front(nodeToStmtNumMap.at(node));
    } else if (isStatement(type)) { // read, print, call, assign
        parentLst = {nodeToStmtNumMap.at(node)};
    }
}

bool ParentExtractor::extract() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        list<string> lst;
        dfs(procNode, lst);
    }
    return true;
}

unordered_map<string, list<string>> ParentExtractor::getParentMap() {
    return this->parentMap;
}

unordered_map<string, list<string>> ParentExtractor::getParentTMap() {
    return this->parentTMap;
}

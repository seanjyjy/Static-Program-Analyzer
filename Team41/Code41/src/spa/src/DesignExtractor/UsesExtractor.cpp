#include "UsesExtractor.h"
#include "Common/TNodeType.h"
#include "PKB/PKB.h"

UsesExtractor::UsesExtractor(TNode *ast, PKB *pkb, unordered_map<TNode *, string> &nodeToStmtNumMap) : ast(ast),
                                                                                                               pkb(pkb), nodeToStmtNumMap(nodeToStmtNumMap) {}

void UsesExtractor::registerUses(TNode *node, list<TNode *> &usesLst) {
    if (node->getType() == TNodeType::procedure) {
        string procName = node->getVal()->getVal();
        for (auto usesNode : usesLst)
            pkb->registerUsesP(procName, usesNode->getVal()->getVal());
    } else {
        string stmtNum = nodeToStmtNumMap.at(node);
        for (auto usesNode : usesLst)
            pkb->registerUsesS(stmtNum, usesNode->getVal()->getVal());
    }
}

void UsesExtractor::dfs(TNode *node, list<TNode *> &usesLst) {
    list<TNode *> usesLstChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], usesLstChild); // only 1 child stmtLst
        usesLst.splice(usesLst.end(), usesLstChild);
        registerUses(node, usesLst);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, usesLstChild);
            usesLst.splice(usesLst.end(), usesLstChild);
            usesLstChild.clear();
        } // no registering for stmtLst
    } else if (type == TNodeType::printStmt) {
        usesLst = {node->getChildren()[0]}; // only 1 child varName
        registerUses(node, usesLst);
    } else if (type == TNodeType::whileStmt || type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesLstChild);
            usesLst.splice(usesLst.end(), usesLstChild);
            usesLstChild.clear();
        }
        registerUses(node, usesLst);
    } else if (type == TNodeType::assignStmt) {
        dfs(node->getChildren()[1], usesLstChild); // right child is expr
        usesLst.splice(usesLst.end(), usesLstChild);
        registerUses(node, usesLst);
    } else if (isCondExpr(type) || isOp(type)) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesLstChild);
            usesLst.splice(usesLst.end(), usesLstChild);
            usesLstChild.clear();
        } // no registering for condExpr and isOp
    } else if (type == TNodeType::varName) {
        usesLst = {node};
    }
}

void UsesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        list<TNode *> lst;
        dfs(procNode, lst);
    }
}

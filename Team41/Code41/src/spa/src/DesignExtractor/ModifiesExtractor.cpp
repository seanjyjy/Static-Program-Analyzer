
#include "ModifiesExtractor.h"
#include "Common/TNodeType.h"
#include "PKB/PKB.h"

ModifiesExtractor::ModifiesExtractor(TNode *ast, PKB *pkb, unordered_map<TNode *, string> &nodeToStmtNumMap) : ast(ast),
                                                                        pkb(pkb), nodeToStmtNumMap(nodeToStmtNumMap) {}

void ModifiesExtractor::registerModifies(TNode *node, list<TNode *> &modifiesLst) {
    if (node->getType() == TNodeType::procedure) {
        string procName = node->getVal()->getVal();
        for (auto modifiesNode : modifiesLst)
            pkb->registerModifiesP(procName, modifiesNode->getVal()->getVal());
    } else {
        string stmtNum = nodeToStmtNumMap.at(node);
        for (auto modifiesNode : modifiesLst)
            pkb->registerModifiesS(stmtNum, modifiesNode->getVal()->getVal());
    }
}

void ModifiesExtractor::dfs(TNode *node, list<TNode *> &modifiesLst) {
    list<TNode *> modifiesLstChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], modifiesLstChild); // only 1 child stmtLst
        modifiesLst.splice(modifiesLst.end(), modifiesLstChild);
        registerModifies(node, modifiesLst);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, modifiesLstChild);
            modifiesLst.splice(modifiesLst.end(), modifiesLstChild);
            modifiesLstChild.clear();
        }
    } else if (type == TNodeType::readStmt || type == TNodeType::assignStmt) {
        modifiesLst = {node->getChildren()[0]}; // left child varName
        registerModifies(node, modifiesLst);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], modifiesLstChild); // right child stmtLst
        modifiesLst.splice(modifiesLst.end(), modifiesLstChild);
        registerModifies(node, modifiesLst);
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], modifiesLstChild);
            modifiesLst.splice(modifiesLst.end(), modifiesLstChild);
            modifiesLstChild.clear();
        }
        registerModifies(node, modifiesLst);
    }
}

void ModifiesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        list<TNode *> lst;
        dfs(procNode, lst);
    }
}

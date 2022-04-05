#include "Common/AstNode/TNodeType.h"
#include <DesignExtractor/EntitiesExtractor.h>
#include <Exception/SemanticException.h>

using namespace std;

EntitiesExtractor::EntitiesExtractor(TNode *ast) : BaseExtractor(ast) {}

void EntitiesExtractor::findProcedures() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        string procName = procNode->getTokenVal();
        if (procSet.find(procName) != procSet.end()) // multiple procedures with same name not allowed
            throw SemanticException("Duplicate Procedure Name '" + procName + "' found");
        procSet.insert(procName);
    }
}

void EntitiesExtractor::recordEntity(TNode *node, int &stmtNum) {
    if (node->isStmt()) {
        stmtNum += 1;
        nodeToStmtNumMap.insert({node, to_string(stmtNum)});
    } else if (node->isVarName()) {
        varSet.insert(node->getTokenVal());
    } else if (node->isConstVal()) {
        constSet.insert(node->getTokenVal());
    }
}

void EntitiesExtractor::dfs(TNode *node, int &stmtNum) {
    recordEntity(node, stmtNum);
    for (TNode *child: node->getChildren())
        dfs(child, stmtNum);
}

bool EntitiesExtractor::extract() {
    try {
        findProcedures();
    } catch (const SemanticException&) {
        return false;
    }
    int stmtNum = 0;
    dfs(ast, stmtNum);
    return true;
}

unordered_map<TNode *, string> EntitiesExtractor::getNodeToStmtNumMap() {
    return this->nodeToStmtNumMap;
}

unordered_set<string> EntitiesExtractor::getProcSet() {
    return this->procSet;
}

unordered_set<string> EntitiesExtractor::getVarSet() {
    return this->varSet;
}

unordered_set<string> EntitiesExtractor::getConstSet() {
    return this->constSet;
}

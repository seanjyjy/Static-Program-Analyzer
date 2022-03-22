#include "Common/TNodeType.h"
#include <DesignExtractor/EntitiesExtractor.h>
#include <Exception/SemanticException.h>

using namespace std;

EntitiesExtractor::EntitiesExtractor(TNode *ast) : BaseExtractor(ast) {}

void EntitiesExtractor::findProcedures() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        string procName = procNode->getTokenVal();
        if (procSet.find(procName) != procSet.end()) // multiple procedures with same name not allowed
            throw SemanticException("Duplicate Procedure Name '" + procName + "' found");
        procSet.insert(procName);
    }
}

void EntitiesExtractor::recordEntity(TNode *node, int &stmtNum) {
    TNodeType type = node->getType();
    if (isStatement(type)) {
        stmtNum += 1;
        nodeToStmtNumMap.insert({node, to_string(stmtNum)});
    } else {
        switch (type) {
            case TNodeType::varName:
                varSet.insert(node->getTokenVal());
                break;
            case TNodeType::constValue:
                constSet.insert(node->getTokenVal());
                break;
        }
    }
}

void EntitiesExtractor::dfs(TNode *node, int &stmtNum) {
    recordEntity(node, stmtNum);
    for (TNode *child: node->getChildren())
        dfs(child, stmtNum);
}

void EntitiesExtractor::extract() {
    findProcedures();
    int stmtNum = 0;
    dfs(ast, stmtNum);
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

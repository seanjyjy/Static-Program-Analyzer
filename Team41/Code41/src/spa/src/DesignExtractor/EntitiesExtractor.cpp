#include <algorithm>
#include <stack>

#include "Common/TNodeType.h"
#include <DesignExtractor/EntitiesExtractor.h>
#include <Exception/SemanticException.h>

using namespace std;

EntitiesExtractor::EntitiesExtractor(TNode *ast) : ast(ast) {}

void EntitiesExtractor::findProcedures() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
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
                varSet.insert(node->getTokenVal()); break;
            case TNodeType::constValue:
                constSet.insert(node->getTokenVal()); break;
        }
    }
}

void EntitiesExtractor::findEntities() {
    int stmtNum = 0;
    stack<TNode *> stk;
    stk.push(ast);
    while (!stk.empty()) {
        TNode *node = stk.top(); stk.pop();
        recordEntity(node, stmtNum);
        vector<TNode *> ch = node->getChildren();
        reverse(ch.begin(), ch.end()); // left to right dfs
        for (TNode *child : ch)
            stk.push(child);
    }
}

void EntitiesExtractor::extractEntities() {
    findProcedures();
    findEntities();
}

unordered_map<TNode *, string> EntitiesExtractor::getNodeToStmtNumMap() {
    return nodeToStmtNumMap;
}

unordered_set<string> EntitiesExtractor::getProcSet() {
    return procSet;
}

unordered_set<string> EntitiesExtractor::getVarSet() {
    return varSet;
}

unordered_set<string> EntitiesExtractor::getConstSet() {
    return constSet;
}

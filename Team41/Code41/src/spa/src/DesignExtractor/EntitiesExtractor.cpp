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
        if (procSet.find(procName) != procSet.end()) // Multiple procedures with same name not allowed
            throw SemanticException("Duplicate Procedure Name '" + procName + "' found");
        procSet.insert(procName);
    }
}

void EntitiesExtractor::recordProcCall(string procCalled, string &procCaller) {
    if (procSet.find(procCalled) == procSet.end()) // Procedure called not in program
        throw SemanticException(procCaller + " called " + procCalled + ", procedure called not in program");
    procCallMap[procCaller].push_back(procCalled);
}

void EntitiesExtractor::recordEntity(TNode *node, int &stmtNum, string &procName) {
    TNodeType type = node->getType();
    if (isStatement(type)) {
        stmtNum += 1;
        nodeToStmtNumMap.insert({node, to_string(stmtNum)});
        if (type == TNodeType::callStmt)
            recordProcCall(node->getChildren()[0]->getTokenVal(), procName); // child is procName node
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
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) { // dfs for every procedure
        string procName = procNode->getTokenVal();
        stack<TNode *> stk;
        stk.push(procNode);
        while (!stk.empty()) {
            TNode *node = stk.top(); stk.pop();
            recordEntity(node, stmtNum, procName);
            vector<TNode *> ch = node->getChildren();
            reverse(ch.begin(), ch.end()); // left to right dfs
            for (TNode *child : ch)
                stk.push(child);
        }
    }
}

void EntitiesExtractor::extractEntities() {
    findProcedures();
    findEntities();
}

unordered_map<TNode *, string> EntitiesExtractor::getNodeToStmtNumMap() {
    return nodeToStmtNumMap;
}

unordered_map<string, list<string>> EntitiesExtractor::getProcCallMap() {
    return procCallMap;
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

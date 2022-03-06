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

void EntitiesExtractor::recordProcCall(const string &procCalled, const string &procCaller) {
    if (procSet.find(procCalled) == procSet.end()) // procedure called not in program
        throw SemanticException(procCaller + " called " + procCalled + ", procedure called not in program");
    procCallMap[procCaller].push_back(procCalled);
}

void EntitiesExtractor::recordEntity(TNode *node, int &stmtNum, const string &procName) {
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

void EntitiesExtractor::cycleCheckCall() {
    unordered_map<string, int> visMap; // UNVISITED = 0, EXPLORED = 1, VISITED = 2
    for (auto it = procCallMap.begin(); it != procCallMap.end(); ++it)
        visMap.insert({it->first, 0}); // all procedures start out as unvisited
    for (auto it = procCallMap.begin(); it != procCallMap.end(); ++it)
        cycleDFS(it->first, visMap);
}

void EntitiesExtractor::cycleDFS(const string &proc, unordered_map<string, int> &visMap) {
    visMap[proc] = 1; // set current proc to EXPLORED
    for (string &childProc : procCallMap[proc]) {
        auto childIt = visMap.find(childProc);
        if (childIt == visMap.end()) // child proc doesn't call anybody
            continue;
        int visNum = childIt->second;
        if (visNum == 0) { // UNVISITED
            cycleDFS(childProc, visMap);
        } else if (visNum == 1) { // EXPLORED
            throw SemanticException("Cyclic procedure call detected");
        }
    }
    visMap[proc] = 2; // set current proc to VISITED
}

void EntitiesExtractor::extractEntities() {
    findProcedures();
    findEntities();
    cycleCheckCall();
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

#include "Common/TNodeType.h"
#include "DesignExtractorUtils.h"
#include "CallsExtractor.h"
#include <Exception/SemanticException.h>

CallsExtractor::CallsExtractor(TNode *ast, unordered_set<string> &procSet) : ast(ast), procSet(procSet) {}

void CallsExtractor::mapCalls(const string &procCalled, const string &procCaller) {
    if (procSet.find(procCalled) == procSet.end()) // procedure called not in program
        throw SemanticException(procCaller + " called " + procCalled + ", procedure called not in program");
    callsMap[procCaller].insert(procCalled);
}

void CallsExtractor::dfs(TNode *node, const string &proc) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], proc); // only 1 child stmtLst
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch)
            dfs(child, proc);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], proc); // right child stmtLst
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], proc);
    } else if (type == TNodeType::callStmt) {
        mapCalls(node->getChildren()[0]->getTokenVal(), proc); // 1 child of procName
    }
}

void CallsExtractor::cycleCheck() {
    unordered_map<string, int> visMap; // UNVISITED = 0, EXPLORED = 1, VISITED = 2
    for (auto it = callsMap.begin(); it != callsMap.end(); ++it)
        visMap.insert({it->first, 0}); // all procedures start out as unvisited
    for (auto it = callsMap.begin(); it != callsMap.end(); ++it)
        cycleCheckDfs(it->first, visMap);
}

void CallsExtractor::cycleCheckDfs(const string &proc, unordered_map<string, int> &visMap) {
    visMap[proc] = 1; // set current proc to EXPLORED
    for (const string &childProc : callsMap[proc]) {
        auto childIt = visMap.find(childProc);
        if (childIt == visMap.end()) // child proc doesn't call anybody
            continue;
        int visNum = childIt->second;
        if (visNum == 0) { // UNVISITED
            cycleCheckDfs(childProc, visMap);
        } else if (visNum == 1) { // EXPLORED
            throw SemanticException("Cyclic procedure call detected");
        }
    }
    visMap[proc] = 2; // set current proc to VISITED
}

void CallsExtractor::revTopoSort() {
    unordered_set<string> visSet;
    for (auto it = callsMap.begin(); it != callsMap.end(); ++it) {
        if (visSet.find(it->first) == visSet.end())
            topoSort(it->first, visSet);
    }
}

void CallsExtractor::topoSort(const string &proc, unordered_set<string> &visSet) {
    visSet.insert(proc);
    for (const string &childProc : callsMap[proc]) {
        auto childIt = callsMap.find(childProc);
        if (childIt == callsMap.end()) // child proc doesn't call anybody
            continue;
        if (visSet.find(childIt->first) == visSet.end()) // child proc not visited
            topoSort(childIt->first, visSet);
    }
    procCallOrder.push_back(proc);
}

void CallsExtractor::buildCallsT() {
    callsTMap = callsMap;
    for (const string& procParent : procCallOrder) {
        auto itParent = callsTMap.find(procParent);
        if (itParent == callsTMap.end()) continue; // parent proc doesn't call(T) anybody
        for (const string& procChild : itParent->second) {
            auto itChild = callsTMap.find(procChild);
            if (itChild == callsTMap.end()) continue; // child proc doesn't call(T) anybody
            DesignExtractorUtils::copyOverSet(itParent->second, itChild->second);
        }
    }
}

void CallsExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        dfs(procNode, procNode->getTokenVal());
    }
    cycleCheck();
    revTopoSort();
    buildCallsT();
}

unordered_map<string, unordered_set<string>> CallsExtractor::getCallsMap() {
    return callsMap;
}

unordered_map<string, unordered_set<string>> CallsExtractor::getCallsTMap() {
    return callsTMap;
}

list<string> CallsExtractor::getProcCallOrder() {
    return procCallOrder;
}

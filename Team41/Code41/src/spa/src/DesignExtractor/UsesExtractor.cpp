#include "UsesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

UsesExtractor::UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                             unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap), callsMap(callsMap), procCallOrder(procCallOrder) {}

void UsesExtractor::mapUses(TNode *node, unordered_set<string> &usesSet) {
    if (usesSet.empty()) return;

    if (node->getType() == TNodeType::procedure) {
        procUsesMap.insert({node->getTokenVal(), usesSet});
    } else {
        stmtUsesMap.insert({nodeToStmtNumMap[node], usesSet});
    }
}

void UsesExtractor::dfs(TNode *node, unordered_set<string> &usesSet) {
    unordered_set<string> usesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], usesSetChild); // only 1 child stmtLst
        usesSet = usesSetChild;
        mapUses(node, usesSet);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (type == TNodeType::printStmt) {
        usesSet = {node->getChildren()[0]->getTokenVal()}; // only 1 child varName
        mapUses(node, usesSet);
    } else if (type == TNodeType::whileStmt || type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
        mapUses(node, usesSet);
    } else if (type == TNodeType::assignStmt) {
        dfs(node->getChildren()[1], usesSetChild); // right child is expr
        usesSet = usesSetChild;
        mapUses(node, usesSet);
    } else if (isCondExpr(type) || isOp(type)) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) { // 1st child is condExpr, rest are stmtLst
            dfs(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (type == TNodeType::varName) {
        usesSet = {node->getTokenVal()};
    } else if (type == TNodeType::callStmt) {
        callNodeSet.insert(node);
    }
}

void UsesExtractor::buildProcUsesCalls() {
    for (const string &procParent : procCallOrder) {
        for (const string& procChild : callsMap[procParent]) { // parent proc should always call a proc
            auto it = procUsesMap.find(procChild);
            if (it == procUsesMap.end()) continue; // child proc doesn't modify anything
            DesignExtractorUtils::copyOverSet(procUsesMap[procParent], it->second);
        }
    }
}

void UsesExtractor::buildCallUses() {
    for (TNode *callNode : callNodeSet) {
        string procCalled = callNode->getChildren()[0]->getTokenVal();
        auto it = procUsesMap.find(procCalled);
        if (it == procUsesMap.end()) continue; // proc called doesn't modify anything
        stmtUsesMap[nodeToStmtNumMap[callNode]] = it->second;
    }
}

void UsesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        unordered_set<string> st;
        dfs(procNode, st);
    }
    buildProcUsesCalls();
    buildCallUses();
}

unordered_map<string, unordered_set<string>> UsesExtractor::getProcUsesMap() {
    return this->procUsesMap;
}

unordered_map<string, unordered_set<string>> UsesExtractor::getStmtUsesMap() {
    return this->stmtUsesMap;
}

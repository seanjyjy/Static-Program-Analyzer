#include "UsesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

UsesExtractor::UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                             unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap), callsMap(callsMap), procCallOrder(procCallOrder) {}

void UsesExtractor::mapUses(TNode *node, unordered_set<string> &usesSet) {
    if (usesSet.empty()) return;

    if (node->getType() == TNodeType::procedure) {
        string procName = node->getTokenVal();
        auto it = procUsesMap.find(procName);
        if (it == procUsesMap.end()) // proc doesn't use anything yet
            procUsesMap.insert({node->getTokenVal(), usesSet});
        else // proc alr uses something
            DesignExtractorUtils::copyOverSet(it->second, usesSet);
    } else {
        string stmtNum = nodeToStmtNumMap[node];
        auto it = stmtUsesMap.find(stmtNum);
        if (it == stmtUsesMap.end()) // stmt doesn't use anything yet
            stmtUsesMap.insert({stmtNum, usesSet});
        else // stmt alr uses something
            DesignExtractorUtils::copyOverSet(it->second, usesSet);
    }
}

void UsesExtractor::dfs(TNode *node, unordered_set<string> &usesSet) {
    unordered_set<string> usesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], usesSet); // only 1 child stmtLst
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

void UsesExtractor::dfsCalls(TNode *node, unordered_set<string> &usesSet) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfsCalls(node->getChildren()[0], usesSet); // only 1 child stmtLst
    } else if (type == TNodeType::stmtLst) {
        unordered_set<string> usesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfsCalls(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
    } else if (type == TNodeType::whileStmt) {
        dfsCalls(node->getChildren()[1], usesSet); // right child stmtLst
        mapUses(node,usesSet);
    } else if (type == TNodeType::ifStmt) {
        unordered_set<string> usesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfsCalls(ch[i], usesSetChild);
            DesignExtractorUtils::combineSetsClear(usesSet, usesSetChild);
        }
        mapUses(node, usesSet);
    }  else if (type == TNodeType::callStmt) {
        string procCalled = node->getChildren()[0]->getTokenVal();
        auto it = procUsesMap.find(procCalled);
        if (it != procUsesMap.end()) // procCalled uses some variables
            usesSet = it->second;
        mapUses(node, usesSet);
    }
}

void UsesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        unordered_set<string> st;
        dfs(procNode, st);
    }
    buildProcUsesCalls();
    for (TNode *procNode : procNodes) { // build stmt uses due to calls
        unordered_set<string> st;
        dfsCalls(procNode, st);
    }
}

unordered_map<string, unordered_set<string>> UsesExtractor::getProcUsesMap() {
    return this->procUsesMap;
}

unordered_map<string, unordered_set<string>> UsesExtractor::getStmtUsesMap() {
    return this->stmtUsesMap;
}

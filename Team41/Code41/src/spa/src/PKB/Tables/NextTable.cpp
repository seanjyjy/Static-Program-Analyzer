#include "NextTable.h"

NextTable::NextTable() = default;

void NextTable::setCFGNode(CFGNode *node, const StmtToNodeMap &map) {
    this->rootCFG = node;
    this->stmtToNodeMap = map;

    if (this->rootCFG == nullptr) {
        return;
    }

    for (auto[stmt, cfgNode]: stmtToNodeMap) {
        vector<CFGNode *> children = cfgNode->getChildren();
        if (!children.empty()) {
            this->allBeforeStmts.push_back(stmt);
        }
        vector<CFGNode *> parents = cfgNode->getParent();
        if (!parents.empty() && parents[0]->getStmtNum() != "0") {
            this->allAfterStmts.push_back(stmt);
        }
        for (auto child: children) {
            this->allPairs.push_back({stmt, child->getStmtNum()});
        }
    }
}

bool NextTable::isNext(const string& stmt1, const string& stmt2) {
    if (stmtToNodeMap.find(stmt1) == stmtToNodeMap.end()) {
        return {};
    }
    for (CFGNode *child: stmtToNodeMap[stmt1]->getChildren()) {
        if (child->getStmtNum() == stmt2) return true;
    }
    return false;
}

vector<CFGNode *> NextTable::getNextNodes(const string& stmt) {
    if (stmtToNodeMap.find(stmt) == stmtToNodeMap.end()) {
        return {};
    }
    return stmtToNodeMap[stmt]->getChildren();
}

vector<CFGNode *> NextTable::getPrevNodes(const string& stmt) {
    if (stmtToNodeMap.find(stmt) == stmtToNodeMap.end()) {
        return {};
    }
    return stmtToNodeMap[stmt]->getParent();
}

vector<pair<string, string>> NextTable::getAllNext() {
    return allPairs;
}

vector<string> NextTable::getAllStmtsExecAfterSomeStmt() {
    return allAfterStmts;
}

vector<string> NextTable::getAllStmtsExecBeforeSomeStmt() {
    return allBeforeStmts;
}

size_t NextTable::getNumOfStartNodes() {
    return allBeforeStmts.size();
}

size_t NextTable::getNumOfEndNodes() {
    return allAfterStmts.size();
}

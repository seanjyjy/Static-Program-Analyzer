#include <algorithm>
#include <iostream>

#include "CFGBuilder.h"

CFGBuilder::CFGBuilder(unordered_map<string, vector<string>> stmtNextMap, vector<string> firstStmts) :
        stmtNextMap(stmtNextMap), firstStmts(firstStmts) {}

int CFGBuilder::stringNumComp(const string &str1, const string &str2) {
    int len1 = str1.length(), len2 = str2.length();
    if (len1 > len2) return 1;
    if (len2 > len1) return -1;

    for (int i = 0; i < len1; ++i) {
        if (str1[i] > str2[i]) return 1;
        if (str1[i] < str2[i]) return -1;
    }
    return 0;
}

void CFGBuilder::revSortChildren() {
    for (auto &[stmtNum, ch] : stmtNextMap) {
        sort(ch.begin(), ch.end(), [this](const string &lhs, const string &rhs) {
            return stringNumComp(lhs, rhs) == -1; // swap if lhs smaller
        });
    }
}

void CFGBuilder::instantiateMap() {
    for (auto &[stmtNum, ch] : stmtNextMap) {
        if (stmtNumToNodeMap.find(stmtNum) != stmtNumToNodeMap.end())
            throw runtime_error("Duplicate stmtNum: " + stmtNum);
        stmtNumToNodeMap[stmtNum] = new CFGNode(stmtNum);
    }
}

void CFGBuilder::addEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode, bool isForward) {
    if (isForward)
        parentCFGNode->addForwardChild(childCFGNode);
    else
        parentCFGNode->addBackwardChild(childCFGNode);
    childCFGNode->addParent(parentCFGNode);
}

void CFGBuilder::buildCFG() {
    for (auto &[stmtNum, ch] : stmtNextMap) {
        bool isForward = true;
        for (const string& child : ch) { // child in decreasing stmtNum
            int cmp = stringNumComp(stmtNum, child);
            if (cmp == 0)
                throw runtime_error(stmtNum + " Next itself");
            if (cmp == 1) // stmtNum bigger than child
                isForward = false;
            addEdge(stmtNumToNodeMap[stmtNum], stmtNumToNodeMap[child], isForward);
        }
    }
    for (const string& stmtNum : firstStmts) // link root to first statement numbers
        cfg->addForwardChild(stmtNumToNodeMap[stmtNum]);
}

void CFGBuilder::build() {
    revSortChildren();
    instantiateMap();
    buildCFG();
}

CFGNode *CFGBuilder::getCFG() {
    return this->cfg;
}

unordered_map<string, CFGNode *> CFGBuilder::getStmtNumToNodeMap() {
    return this->stmtNumToNodeMap;
}

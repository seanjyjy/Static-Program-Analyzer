#include <algorithm>
#include <iostream>

#include "CFGBuilder.h"

CFGBuilder::CFGBuilder(unordered_map<string, vector<string>> stmtNextMap, vector<string> firstStmts) :
        stmtNextMap(stmtNextMap), firstStmts(firstStmts) {}

void CFGBuilder::instantiateMap() {
    stmtNumToNodeMap["0"] = cfg;
    for (auto &[stmtNum, ch]: stmtNextMap)
        stmtNumToNodeMap[stmtNum] = new CFGNode(stmtNum);
}

void CFGBuilder::addEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode) {
    parentCFGNode->addChild(childCFGNode);
    childCFGNode->addParent(parentCFGNode);
}

void CFGBuilder::buildCFG() {
    for (auto &[stmtNum, ch]: stmtNextMap) {
        for (const string &child: ch) { // child in decreasing stmtNum
            if (stmtNum == child)
                throw runtime_error(stmtNum + " Next itself");
            addEdge(stmtNumToNodeMap[stmtNum], stmtNumToNodeMap[child]);
        }
    }
    for (const string &stmtNum: firstStmts) // link root to first statement numbers
        cfg->addChild(stmtNumToNodeMap[stmtNum]);
}

void CFGBuilder::build() {
    instantiateMap();
    buildCFG();
}

CFGNode *CFGBuilder::getCFG() {
    return this->cfg;
}

unordered_map<string, CFGNode *> CFGBuilder::getStmtNumToNodeMap() {
    return this->stmtNumToNodeMap;
}

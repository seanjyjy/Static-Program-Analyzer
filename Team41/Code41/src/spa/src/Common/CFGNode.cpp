#include <iostream>
#include <queue>
#include <unordered_set>
#include <cassert>

#include "CFGNode.h"

CFGNode::CFGNode(string stmtNum) : stmtNum(stmtNum) {}

void CFGNode::addChild(CFGNode *node) {
    children.push_back(node);
}

void CFGNode::addParent(CFGNode *node) {
    parent.push_back(node);
}

string CFGNode::getStmtNum() {
    return stmtNum;
}

std::vector<CFGNode *> CFGNode::getChildren() {
    return children;
}

std::vector<CFGNode *> CFGNode::getParent() {
    return parent;
}

void CFGNode::printCFG(CFGNode *node) {
    std::unordered_set<CFGNode *> seen;
    std::queue<CFGNode *> q;
    q.push(node);
    seen.insert(node);
    while (!q.empty()) {
        node = q.front(); q.pop();
        string stmtNum = node->getStmtNum();
        std::cout << stmtNum << ": ";
        for (CFGNode *child : node->getChildren()) {
            std::cout << child->stmtNum << ", ";
            if (seen.find(child) != seen.end()) continue;
            q.push(child);
            seen.insert(child);
        }
        std::cout << endl;
    }
}

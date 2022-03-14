#include <iostream>
#include <queue>
#include <unordered_set>

#include "CFGNode.h"

CFGNode::CFGNode(string stmtNum) : stmtNum(stmtNum) {}

CFGNode::~CFGNode() {
    // all forward children are added before backward children, so only need delete forward children
    for (int i = 0; i < numForward; i++)
        delete this->children[i];
}

void CFGNode::addForwardChild(CFGNode *node) {
    this->children.push_back(node);
    ++this->numForward;
}

void CFGNode::addBackwardChild(CFGNode *node) {
    this->children.push_back(node);
}

void CFGNode::addParent(CFGNode *node) {
    this->parent.push_back(node);
}

string CFGNode::getStmtNum() {
    return this->stmtNum;
}

std::vector<CFGNode *> CFGNode::getChildren() {
    return this->children;
}

std::vector<CFGNode *> CFGNode::getParent() {
    return this->parent;
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

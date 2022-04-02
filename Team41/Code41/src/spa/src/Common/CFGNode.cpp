#include <queue>
#include <unordered_set>

#include "CFGNode.h"

CFGNode::CFGNode(string stmtNum) : stmtNum(stmtNum) {}

CFGNode::~CFGNode() = default;

void CFGNode::addChild(CFGNode *node) {
    children.push_back(node);
}

void CFGNode::addParent(CFGNode *node) {
    parent.push_back(node);
}

string CFGNode::getStmtNum() {
    return stmtNum;
}

const std::vector<CFGNode *> &CFGNode::getChildren() {
    return children;
}

const std::vector<CFGNode *> &CFGNode::getParent() {
    return parent;
}

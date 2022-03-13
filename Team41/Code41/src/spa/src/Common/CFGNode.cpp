#include <iostream>
#include <queue>
#include <unordered_set>

#include "CFGNode.h"

CFGNode::CFGNode(TNode *tNode) : tNode(tNode) {}

// TODO: delete method here

void CFGNode::addChild(CFGNode *node) {
    this->children.push_back(node);
}

TNode *CFGNode::getTNode() {
    return this->tNode;
}

std::vector<CFGNode *> CFGNode::getChildren() {
    return this->children;
}

void CFGNode::printCFG(CFGNode *node, std::unordered_map<TNode *, string> &nodeToStmtNumMap) {
    std::unordered_set<CFGNode *> seen;
    std::queue<CFGNode *> q;
    q.push(node);
    seen.insert(node);
    while (!q.empty()) {
        node = q.front(); q.pop();
        if (node->getTNode()) // will skip root cfg node
            std::cout << nodeToStmtNumMap[node->getTNode()] << ": ";
        for (CFGNode *child : node->getChildren()) {
            std::cout << nodeToStmtNumMap[child->getTNode()] << ", ";
            if (seen.find(child) != seen.end()) continue;
            q.push(child);
            seen.insert(child);
        }
        std::cout << endl;
    }
}

#pragma once

#include <unordered_map>
#include <vector>

#include "TNode.h"

class CFGNode {
private:
    string stmtNum; // statement number that corresponds to this cfg node
    std::vector<CFGNode *> children;
    std::vector<CFGNode *> parent;

public:
    CFGNode(string stmtNum);
    ~CFGNode();

    void addChild(CFGNode *node);

    void addParent(CFGNode *node);

    string getStmtNum();

    const std::vector<CFGNode *> &getChildren();

    const std::vector<CFGNode *> &getParent();
};

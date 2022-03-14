#pragma once

#include <unordered_map>
#include <vector>

#include "TNode.h"

class CFGNode {
private:
    string stmtNum; // statement number that corresponds to this cfg node
    int numForward = 0; // number of children that are forward (stmt number higher)
    std::vector<CFGNode *> children;
    std::vector<CFGNode *> parent;

public:
    CFGNode(string stmtNum);
    ~CFGNode();

    void addForwardChild(CFGNode *node);

    void addBackwardChild(CFGNode *node);

    void addParent(CFGNode *node);

    string getStmtNum();

    std::vector<CFGNode *> getChildren();

    std::vector<CFGNode *> getParent();

    static void printCFG(CFGNode *node); // print CFG for troubleshooting
};

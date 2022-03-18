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
    //~CFGNode(); TODO

    void addChild(CFGNode *node);

    void addParent(CFGNode *node);

    string getStmtNum();

    std::vector<CFGNode *> getChildren();

    std::vector<CFGNode *> getParent();

    static void printCFG(CFGNode *node); // print CFG for troubleshooting
};

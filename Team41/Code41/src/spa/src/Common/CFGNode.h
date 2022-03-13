#pragma once

#include <unordered_map>
#include <vector>

#include "TNode.h"

class CFGNode {
private:
    TNode *tNode; // ast stmt node that corresponds to this cfg node
    std::vector<CFGNode *> children;
    std::vector<CFGNode *> parent;

public:
    CFGNode(TNode *tNode);
    //~CFGNode();

    void addChild(CFGNode *node);

    void addParent(CFGNode *node);

    TNode *getTNode();

    string getStmtNum();

    std::vector<CFGNode *> getChildren();

    std::vector<CFGNode *> getParent();

    // print CFG for troubleshooting
    static void printCFG(CFGNode *node, std::unordered_map<TNode *, string> &nodeToStmtNumMap);
};

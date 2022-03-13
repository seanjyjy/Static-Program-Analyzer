#pragma once

#include <unordered_map>
#include <vector>

#include "TNode.h"

class CFGNode {
private:
    TNode *tNode; // ast stmt node that corresponds to this cfg node
    std::vector<CFGNode *> children;

public:
    CFGNode(TNode *tNode);
    //~CFGNode();

    void addChild(CFGNode *node);

    TNode *getTNode();

    std::vector<CFGNode *> getChildren();

    // print CFG for troubleshooting
    static void printCFG(CFGNode *node, std::unordered_map<TNode *, string> &nodeToStmtNumMap);
};

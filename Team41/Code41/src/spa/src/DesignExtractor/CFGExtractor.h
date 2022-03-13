#pragma once

#include <vector>
#include <unordered_map>

#include "Common/CFGNode.h"
#include "Common/TNode.h"

class CFGExtractor {
private:
    TNode *ast; // root node of AST
    CFGNode *cfg = new CFGNode(nullptr); // root node of CFG
    unordered_map<TNode *, CFGNode *> tNodeToCFGNodeMap; // mapping of TNode* to CFG*

    /**
     * Creates CFGNode using the TNode, and adds to the TNode* to CFGNode* mapping.
     *
     * @param tNode node of the AST to create CFGNode
     * @return CFG Node created
     */
    CFGNode *createCFGNode(TNode *tNode);

    /**
     * Traverses through AST in BFS manner to build the initial CFG,
     * which does not contain edges back due to WHILE loops.
     */
    void buildInitCFG();

    /**
     * Traverses through AST to add back edges due to WHILE loops to the initial CFG.
     */
    void linkBackNode();

public:
    CFGExtractor(TNode *ast);

    /**
     * Builds CFG.
     */
    void extractCFG();

    /**
     * @return Root of CFG, whose inner TNode is nullptr and children are first stmts of each procedure.
     */
    CFGNode *getCFG();
};

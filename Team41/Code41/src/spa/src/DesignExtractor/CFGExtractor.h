#pragma once

#include <vector>
#include <unordered_map>

#include "Common/CFGNode.h"
#include "Common/TNode.h"

class CFGExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    CFGNode *cfg = new CFGNode("0"); // root node of CFG
    unordered_map<string, CFGNode *> stmtNumToNodeMap; // mapping of statement number to CFGNode*

    /**
     * Creates CFGNode using the TNode, and adds to the TNode* to CFGNode* mapping.
     *
     * @param tNode node of the AST to create CFGNode
     * @return CFG Node created
     */
    CFGNode *createCFGNode(TNode *tNode);

    /**
     * Adds edge between 2 CFGNodes, both child and parent edges.
     *
     * @param parentCFGNode
     * @param childCFGNode
     */
    void addCFGEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode);

    /**
     * Traverses through AST in BFS manner to build the initial CFG,
     * which does not contain edges back due to WHILE loops.
     */
    void buildInitCFG();

    /**
     * Adds an edge in the CFG, from CFGNode that corresponds to one TNode to another CFGNode that corresponds
     * to another TNode. The edge is from higher stmtNum to lower stmtNum.
     *
     * @param fromTNode TNode that edge is from
     * @param toTNode TNode that edge is towards
     */
    void addBackEdge(TNode *fromTNode, TNode *toTNode);

    /**
     * Traverses through AST to add back edges due to WHILE loops to the initial CFG.
     */
    void linkBackNode();

public:
    CFGExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Builds CFG.
     */
    void extractCFG();

    /**
     * @return Root of CFG, whose inner TNode is nullptr and children are first stmts of each procedure
     */
    CFGNode *getCFG();

    /**
     * @return Mapping of statement number to CFGNode it corresponds to
     */
    unordered_map<string, CFGNode *> getStmtNumToNodeMap();
};

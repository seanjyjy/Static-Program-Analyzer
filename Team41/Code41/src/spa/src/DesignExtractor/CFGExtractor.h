#pragma once

#include <vector>
#include <unordered_map>

#include "Base/StmtNumExtractor.h"
#include "Common/CFGNode.h"
#include "Common/TNode.h"

class CFGExtractor: public StmtNumExtractor {
private:
    const string ROOT_INDEX = "0";
    CFGNode *cfg = new CFGNode(ROOT_INDEX); // root node of CFG
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
     * Traverses through AST to build the initial CFG, which does not contain edges back due to WHILE loops.
     */
    void dfsInitCFG(TNode *curTNode, CFGNode *curCFGNode, CFGNode *parentCFGNode);

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
    void dfsLinkBack(TNode *curTNode, TNode *backTNode);

public:
    CFGExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Builds CFG.
     */
    void extract() override;

    /**
     * @return Root of CFG, whose inner TNode is nullptr and children are first stmts of each procedure
     */
    CFGNode *getCFG();

    /**
     * @return Mapping of statement number to CFGNode it corresponds to
     */
    unordered_map<string, CFGNode *> getStmtNumToNodeMap();
};

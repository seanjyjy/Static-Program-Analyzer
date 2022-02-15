#pragma once

#include <list>
#include <unordered_map>

#include "Common/TNode.h"

class PatternExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, pair<string, TNode *>> assignPatternMap; // mapping of statement number of assign statement to
                                                                   // pair of <lhs variable, TNode* of rhs of assign>

    /**
     * Records current assign statement to pair <lhs variable, TNode* of rhs>.
     *
     * @param node current TNode of AST which is an assignment
     */
    void mapAssignPattern(TNode *node);

    /**
     * Traverses through AST to record assign patterns.
     *
     * @param node current TNode being processed in tree traversal
     */
    void dfs(TNode *node);

public:
    PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Records all assign patterns in assignPatternMap.
     */
    void extractRelationship();

    unordered_map<string, pair<string, TNode *>> getAssignPatternMap();
};

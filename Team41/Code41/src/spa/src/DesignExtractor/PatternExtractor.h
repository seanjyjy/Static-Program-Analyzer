#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class PatternExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, pair<string, TNode *>> assignPatternMap; // mapping of statement number of assign statement to
                                                                   // pair of <lhs variable, TNode* of rhs of assign>
    unordered_map<string, unordered_set<string>> ifPatternMap; // mapping of statement number of if statement to set of condVariables
    unordered_map<string, unordered_set<string>> whilePatternMap; // mapping of statement number of while statement to set of condVariables

    /**
     * Records current assign statement to pair <lhs variable, TNode* of rhs>.
     *
     * @param node current TNode of AST which is an assignment
     */
    void mapAssignPattern(TNode *node);

    /**
     * Records current if statement to condition variable.
     *
     * @param node current TNode of AST which is an if stmt
     * @param varSet set containing variables are used in condition of current if statement
     */
    void mapIfPattern(TNode *node, unordered_set<string> &varSet);

    /**
     * Records current assign statement to condition variable.
     *
     * @param node current TNode of AST which is a while stmt
     * @param varSet set containing variables are used in condition of current while statement
     */
    void mapWhilePattern(TNode *node, unordered_set<string> &varSet);

    /**
     * Traverses through AST to record patterns.
     *
     * @param node current TNode being processed in tree traversal
     */
    void dfs(TNode *node);

    /**
     * Traverses through expression AST to get set of variables.
     *
     * @param node current TNode expression being processed in tree traversal
     * @param usesSet set containing variables are used by expression
     */
    void dfsExpr(TNode *node, unordered_set<string> &varSet);

public:
    PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Records all patterns.
     */
    void extractRelationship();

    unordered_map<string, pair<string, TNode *>> getAssignPatternMap();

    unordered_map<string, unordered_set<string>> getIfPatternMap();

    unordered_map<string, unordered_set<string>> getWhilePatternMap();
};

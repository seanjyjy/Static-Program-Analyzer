#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class UsesExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, unordered_set<string>> procUsesMap; // mapping of procedure to set of variables that are used
    unordered_map<string, unordered_set<string>> stmtUsesMap; // mapping of statement to set of variables that are used

    /**
     * Records current procedure/statement to set of variables that it uses.
     *
     * @param node current TNode of AST which is a procedure/statement
     * @param usesSet set of variables that are used by the procedure/statement
     */
    void mapUses(TNode *node, unordered_set<string> &usesSet);

    /**
     * Traverses through AST, building uses set upwards and recording them for procedures and statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param usesSet set containing variables are used by current procedure/statement
     */
    void dfs(TNode *node, unordered_set<string> &usesSet);

public:
    UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Records all uses relationship in procUsesMap and stmtUsesMap.
     */
    void extractRelationship();

    unordered_map<string, unordered_set<string>> getProcUsesMap();
    unordered_map<string, unordered_set<string>> getStmtUsesMap();
};

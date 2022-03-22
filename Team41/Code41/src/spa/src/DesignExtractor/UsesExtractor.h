#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class UsesExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, unordered_set<string>> &callsMap; // mapping of proc to list of proc it directly calls
    list<string> &procCallOrder; // list of procedures in reversed toposort order of calls graph
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

    /**
     * Adds variables used by proc due to call stmts in the proc.
     */
    void buildProcUsesCalls();

    /**
    * Records stmts to set of variables it uses, based on the set of variables the procedures called uses.
    */
    void dfsCalls(TNode *node, unordered_set<string> &usesSet);

public:
    UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                  unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder);

    /**
     * Records all uses relationship in procUsesMap and stmtUsesMap.
     */
    void extractRelationship();

    unordered_map<string, unordered_set<string>> getProcUsesMap();
    unordered_map<string, unordered_set<string>> getStmtUsesMap();
};

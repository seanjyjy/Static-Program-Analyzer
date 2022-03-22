#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class ModifiesExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, unordered_set<string>> &callsMap; // mapping of proc to list of proc it directly calls
    list<string> &procCallOrder; // list of procedures in reversed toposort order of calls graph
    unordered_map<string, unordered_set<string>> procModifiesMap; // mapping of procedure to set of variables that are modified
    unordered_map<string, unordered_set<string>> stmtModifiesMap; // mapping of statement to set of variables that are modified

    /**
     * Records current procedure/statement to set of variables that it modifies.
     *
     * @param node current TNode of AST which is a procedure/statement
     * @param modifiesSet set of variables that are modified by the procedure/statement
     */
    void mapModifies(TNode *node, unordered_set<string> &modifiesSet);

    /**
     * Traverses through AST, building modifies set upwards and recording them for procedures and statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param modifiesSet set containing variables are modified by current procedure/statement
     */
    void dfs(TNode *node, unordered_set<string> &modifiesSet);

    /**
     * Adds variables modified by proc due to call stmts in the proc.
     */
    void buildProcModifiesCalls();

    /**
     * Records stmts to set of variables it modifies, based on the set of variables the procedures called modifies.
     */
    void dfsCalls(TNode *node, unordered_set<string> &modifiesSet);

public:
    ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                      unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder);

    /**
     * Records all modifies relationship in procModifiesMap and stmtModifiesMap.
     */
    void extractRelationship();

    unordered_map<string, unordered_set<string>> getProcModifiesMap();
    unordered_map<string, unordered_set<string>> getStmtModifiesMap();
};

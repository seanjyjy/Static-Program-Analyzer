#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"
#include "StmtNumExtractor.h"

/**
 * Abstract class for extractor that do Variable Relation (Currently Modifies and Uses)
 */
class VarRelationExtractor : public StmtNumExtractor {
protected:
    const unordered_map<string, unordered_set<string>> &callsMap; // mapping of proc to list of proc it directly calls
    const list<string> &procCallOrder; // list of procedures in reversed toposort order of calls graph
    unordered_map<string, unordered_set<string>> procRelationMap; // mapping of procedure to set of variables that <relation>
    unordered_map<string, unordered_set<string>> stmtRelationMap; // mapping of statement to set of variables that <relation>

    /**
     * Records current procedure/statement to set of variables that it <relation>.
     *
     * @param node current TNode of AST which is a procedure/statement
     * @param modifiesSet set of variables that <relation> by the procedure/statement
     */
    void mapRelation(TNode *node, unordered_set<string> &relationSet);

    /**
     * Traverses through AST, building relation set upwards and recording them for procedures and statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param relationSet set containing variables are <relation> by current procedure/statement
     */
    virtual void dfs(TNode *node, unordered_set<string> &relationSet) = 0;

    /**
     * Adds variables modified by <relation> due to call stmts in the proc.
     */
    void buildProcRelationCalls();

    /**
     * Records stmts to set of variables it <relation>, based on the set of variables the procedures called <relation>.
     */
    void dfsCalls(TNode *node, unordered_set<string> &relationSet);

public:
    VarRelationExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                         unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder);

    /**
     * Records all <relation> relationship in procRelationMap and stmtRelationMap.
     */
    void extract() override;

    unordered_map<string, unordered_set<string>> getProcRelationMap();

    unordered_map<string, unordered_set<string>> getStmtRelationMap();
};

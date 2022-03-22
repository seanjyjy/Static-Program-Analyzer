#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Base/VarRelationExtractor.h"
#include "Common/TNode.h"

class ModifiesExtractor : public VarRelationExtractor {
private:
    /**
     * Traverses through AST, building modifies set upwards and recording them for procedures and statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param modifiesSet set containing variables are modified by current procedure/statement
     */
    void dfs(TNode *node, unordered_set<string> &modifiesSet) override;

public:
    ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                      unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder);

    /**
     * Records all modifies relationship in procRelationMap and stmtRelationMap.
     */
    void extract() override;

    unordered_map<string, unordered_set<string>> getProcModifiesMap();

    unordered_map<string, unordered_set<string>> getStmtModifiesMap();
};

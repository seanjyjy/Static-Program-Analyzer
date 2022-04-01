#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>

#include "Base/VarRelationExtractor.h"
#include "Common/TNode.h"

class UsesExtractor : public VarRelationExtractor {
private:
    /**
     * Traverses through AST, building uses set upwards and recording them for procedures and statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param usesSet set containing variables are used by current procedure/statement
     */
    void dfs(TNode *node, unordered_set<string> &usesSet) override;

public:
    UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                  unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder);

    /**
     * Records all uses relationship in procRelationMap and stmtRelationMap.
     */
    bool extract() override;

    unordered_map<string, unordered_set<string>> getProcUsesMap();

    unordered_map<string, unordered_set<string>> getStmtUsesMap();
};

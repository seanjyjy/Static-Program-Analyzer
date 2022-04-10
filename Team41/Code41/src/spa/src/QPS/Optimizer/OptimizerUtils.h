#pragma once

#include "QPS/QO/SuperClause.h"
#include "QPS/QO/Selectable.h"

/**
 * Utility class for miscellaneous functions used by the query optimizer.
 */
class OptimizerUtils {
public:
    /**
     * Returns true if any synonym in the given query declarations is found in the given clauses, false otherwise.
     */
    static bool hasSynonymOverlap(const vector<QueryDeclaration> &qds, const vector<SuperClause *> &clauses);

    /**
     * Removes duplicate clauses. Is stable - does not reorder clauses.
     */
    static vector<SuperClause *> removeDuplicates(const vector<SuperClause *> &clauses);

    /**
     * Prints a representation of the given groups of clauses. Mainly for debugging.
     */
    static void print(vector<vector<SuperClause *>> &clauses);
};

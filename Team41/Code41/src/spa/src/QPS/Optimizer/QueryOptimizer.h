#pragma once

#include "QPS/QO/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/QO/SuperClause.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "OptimizerUtils.h"
#include "ClauseGroups.h"
#include "SortedGroups.h"
#include "FifoGroups.h"
#include "SortedGroup.h"
#include "FifoGroup.h"
#include "PKBAwareGroup.h"

using namespace std;

/**
 * The query optimizer takes in a query object, optimizes it and returns an optimized query object.
 * Optimization is configurable; configuration options that don't require additional dependencies such as
 * the PKB are turned on by default.
 *
 * Example usage:
 *
 * // Create the optimizer and optimize a plain query object.
 * OptimizedQueryObject oqo = QueryOptimizer::create()
 *                              .setClauseGrouping(true)
 *                              .setInterGroupSort(true)
 *                              .setIntraGroupSort(true)
 *                              .enableDynamicPolling(&pkbManager)
 *                              .optimize(queryObject);
 * // Print a description of the optimized query object
 * oqo.print();
 *
 * Configuration options:
 *
 * 1. Clause Grouping
 *  - groups clauses that are linked to one another via their synonyms.
 *  - when turned off, there is just one group (containing all clauses in the original query object).
 *  - turned ON by default.
 *  - examples: see TestClauseDepGraph.cpp in unit tests
 *
 * 2. Inter-clause Grouping
 *  - sorts clause groups based on their group score - see ClauseScorer for clause scoring details.
 *  - turned ON by default.
 *
 * 3. Intra-clause Grouping
 *  - sorts clauses with groups based on their individual score - see ClauseScorer for clause scoring details
 *  - turned ON by default.
 *
 * 4. Dynamic Polling
 *  - greedily returns the next clause that results in the smallest intermediate table.
 *  - turned ON by default.
 *
 * 5. Duplicate clause removal
 *  - removes duplicate clauses; guarantees each clause popped will be unique.
 *  - turned ON by default.
 *
 * 6. Clause group simplification
 *  - marks clause groups as simplifiable if all clauses in the group don't share synonyms with the synonyms selected
 *  in the PQL query.
 *  - turned ON by default.
 *  - examples: see TestQueryOptimizerOptimizations.cpp in unit tests
 */
class QueryOptimizer {
private:
    // default configuration options
    bool isClauseGroupingEnabled = true;
    bool isSimplifyGroupsEnabled = true;
    bool isIntraGroupSortEnabled = true;
    bool isInterGroupSortEnabled = true;
    bool isDynamicPollingEnabled = false;
    bool isDupClauseRemovalEnabled = true;
    bool isPkbAdapterSet = false;
    PKBAdapter adapter;
    ClauseDepGraph clauseDepGraph;

    QueryOptimizer();

public:

    static QueryOptimizer create();

    // convenience method to turn on all forms of optimization
    QueryOptimizer &enableAllOptimizations(PKBManager *pkbManager);

    // if turned on, sorts clause groups based on clause group score
    QueryOptimizer &setIntraGroupSort(bool isOn);

    // if turned on, sorts clauses in a clause group based on clause score
    QueryOptimizer &setInterGroupSort(bool isOn);

    // if turned on, groups clauses based on dependencies in their synonyms
    QueryOptimizer &setClauseGrouping(bool isOn);

    // if turned on, marks clause groups as simplifiable if clauses in the group don't overlap with selectables
    QueryOptimizer &setSimplifyGroups(bool isOn);

    // if turned on, removes duplicate clauses and ensures all clauses popped will be unique.
    QueryOptimizer &setDupClauseRemoval(bool isOn);

    // if turned on, queries the PKB and retrieves the next clause that produces a table with the least rows
    QueryOptimizer &enableDynamicPolling(PKBManager *pkbManager);

    // optimizes the given query object based on optimization flags
    OptimizedQueryObject optimize(QueryObject *qo);

    // prints the configuration options of the optimizer. for debugging purposes.
    void print();
};

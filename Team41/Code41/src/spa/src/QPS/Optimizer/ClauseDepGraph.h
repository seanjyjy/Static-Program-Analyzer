#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "QPS/QO/QueryClause.h"
#include "QPS/QO/PatternClause.h"
#include "QPS/Optimizer/SimpleGraph.h"
#include "QPS/QO/SuperClause.h"

using namespace std;

/**
 * Represents a clause dependency graph. Used to group clauses with the same synonym dependencies.
 */
class ClauseDepGraph {
private:
    // used to tell if a clause has been processed (added to a clause group)
    using TaggedSuperClause = pair<SuperClause *, int>;

    // used to identify the group of clauses with no synonyms
    const string NO_SYNONYM = "_NO_SYNONYMS";

    // for grouping clauses by clause synonym dependencies
    int clauseId = 0;

    // used to group synonyms of clauses with their string representation
    SimpleGraph<string> graph;

    // maps synonyms to clauses with that synonym
    unordered_map<string, vector<TaggedSuperClause>> synonymToClauses;

    // clause id -> is clause processed. prevents duplicate clauses in clause groups.
    unordered_map<int, bool> isCidProcessed;

    // convenience method to get the next clauseId and update it
    int getCid();

    // convenience method to retrieve only unprocessed clauses
    vector<SuperClause *> getUnprocessedClauses(vector<TaggedSuperClause> &clauses);

public:
    ClauseDepGraph();

    /**
     * Register a clause for grouping. Each clause should only be registered once.
     */
    void registerClause(SuperClause *cl);

    /**
     * Returns the grouping of clauses based on their synonym dependencies.
     * Should be called only after all clauses to be grouped have been registered.
     */
    vector<vector<SuperClause *>> split();
};

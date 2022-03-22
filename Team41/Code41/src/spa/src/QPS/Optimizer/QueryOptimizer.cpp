#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject *qo) {
    // do some preliminary checks
    if (isDynamicPollingEnabled && !isPkbAdapterSet) throw runtime_error("pkb adapter not set");

    // first initialize the groups container
    AbstractGroups *groups = nullptr;
    if (isInterGroupSortEnabled) {
        groups = new SortedGroups();
    } else {
        groups = new FifoGroups();
    }

    // step 1: Take query object, extract clauses, group them based on synonym dependencies
    vector<vector<SuperClause *>> clauseGroups;
    if (isClauseGroupingEnabled) {
        // clause grouping enabled - form dependency graph and split clauses
        for (SuperClause *cl: qo->getSuperClauses()) {
            clauseDepGraph.registerClause(cl);
        }
        clauseGroups = clauseDepGraph.split();
    } else {
        // no clause grouping - just one group of all clauses
        clauseGroups.push_back(qo->getSuperClauses());
    }

    // step 2: Add clause groups
    for (vector<SuperClause *> &clauseGroup: clauseGroups) {
        clauseGroup = OptimizerUtils::removeDuplicates(clauseGroup);

        if (isIntraGroupSortEnabled && isDynamicPollingEnabled) {
            groups->addGroup(new PKBAwareGroup(clauseGroup, adapter));
        } else if (isIntraGroupSortEnabled) {
            groups->addGroup(new SortedGroup(clauseGroup));
        } else {
            groups->addGroup(new FifoGroup(clauseGroup));
        }
    }

    return {qo, groups};
}

QueryOptimizer QueryOptimizer::create() {
    return QueryOptimizer{};
}



void QueryOptimizer::print() {
    cout << "isClauseGroupingEnabled: " << isClauseGroupingEnabled << endl
         << "isIntraGroupSortEnabled: " << isIntraGroupSortEnabled << endl
         << "isInterGroupSortEnabled: " << isInterGroupSortEnabled << endl
         << "isDynamicPollingEnabled: " << isDynamicPollingEnabled << endl;
}

QueryOptimizer &QueryOptimizer::setInterGroupSort(bool isOn) {
    isInterGroupSortEnabled = isOn;
    return *this;
}

QueryOptimizer &QueryOptimizer::setIntraGroupSort(bool isOn) {
    isIntraGroupSortEnabled = isOn;
    return *this;
}

QueryOptimizer &QueryOptimizer::setClauseGrouping(bool isOn) {
    isClauseGroupingEnabled = isOn;
    return *this;
}

QueryOptimizer &QueryOptimizer::enableDynamicPolling(const PKBAdapter &pkbAdapter) {
    adapter = pkbAdapter;
    isDynamicPollingEnabled = true;
    isPkbAdapterSet = true;
    return *this;
}

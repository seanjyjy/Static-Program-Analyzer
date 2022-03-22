#include "QueryOptimizer.h"

#include <utility>

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject *qo) {
    // do some preliminary checks
    if (isDynamicPollingEnabled && !isPkbAdapterSet) throw runtime_error("pkb adapter not set");

//    AbstractGroups *groups = nullptr;
//    if (isInterGroupSortEnabled) {
//        groups = new SortedGroups();
//    } else {
//        groups = new FifoGroups();
//    }

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
    vector<AbstractGroup*> groups;
    for (vector<SuperClause *> &clauseGroup: clauseGroups) {
        clauseGroup = OptimizerUtils::removeDuplicates(clauseGroup);

        if (isIntraGroupSortEnabled && isDynamicPollingEnabled) {
            groups.push_back(new PKBAwareGroup(clauseGroup, adapter));
        } else if (isIntraGroupSortEnabled) {
            groups.push_back(new SortedGroup(clauseGroup));
        } else {
            groups.push_back(new FifoGroup(clauseGroup));
        }
    }

    // initialize the groups container
    if (isInterGroupSortEnabled) {
        return {qo, new SortedGroups(groups)};
    } else {
        return {qo, new FifoGroups(groups)};
    }
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

QueryOptimizer &QueryOptimizer::enableDynamicPolling(PKBManager *pkbManager) {
    // for some reason, accepting a const reference to pkbmanager causes some error >:(
    adapter = PKBAdapter(pkbManager);
    isDynamicPollingEnabled = true;
    isPkbAdapterSet = true;
    return *this;
}

#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject *qo) {
    // do some preliminary checks
    if (isDynamicPollingEnabled && !isPkbAdapterSet) throw runtime_error("pkb adapter not set");

    bool hasError = !qo->isValid() || qo->hasUseOfUndeclaredVariable();

    // step 1: Take query object, extract clauses, group them based on synonym dependencies
    vector<vector<SuperClause *>> clauseGroups;
    if (isClauseGroupingEnabled && !hasError) {
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
    for (vector<SuperClause *> &clauses: clauseGroups) {
        if (isDupClauseRemovalEnabled) clauses = OptimizerUtils::removeDuplicates(clauses);

        if (isIntraGroupSortEnabled && isDynamicPollingEnabled && !hasError) {
            groups.push_back(new PKBAwareGroup(clauses, adapter));
        } else if (isIntraGroupSortEnabled && !hasError) {
            groups.push_back(new SortedGroup(clauses));
        } else {
            groups.push_back(new FifoGroup(clauses));
        }
    }

    // initialize the groups container
    if (isInterGroupSortEnabled && !hasError) {
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

QueryOptimizer &QueryOptimizer::setDupClauseRemoval(bool isOn) {
    isDupClauseRemovalEnabled = isOn;
    return *this;
}

QueryOptimizer &QueryOptimizer::enableAllOptimizations(PKBManager *pkbManager) {
    return setClauseGrouping(true)
            .setIntraGroupSort(true)
            .setInterGroupSort(true)
            .setDupClauseRemoval(true)
            .enableDynamicPolling(pkbManager);
}

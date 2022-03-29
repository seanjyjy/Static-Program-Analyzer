#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject *qo) {
    // if there are errors in the query object, don't do anything
    if (!qo->isValid() || qo->hasUseOfUndeclaredVariable()) return {qo};

    // preliminary checks: dynamic polling requires pkb adapter
    if (isDynamicPollingEnabled && !isPkbAdapterSet) throw runtime_error("pkb adapter not set");

    // step 1: Take query object, extract clauses, group them based on synonym dependencies
    vector<vector<SuperClause *>> clauseGroups;
    if (isClauseGroupingEnabled) {
        // clause grouping enabled - form dependency graph and split clauses
        for (SuperClause *cl: qo->getSuperClauses()) {
            clauseDepGraph.registerClause(cl);
        }
        // turn on group simplification if needed
        clauseGroups = clauseDepGraph.split();
    } else {
        // no clause grouping - just one group of all clauses
        clauseGroups.push_back(qo->getSuperClauses());
    }

    // step 2: Add clause groups
    vector < ClauseGroup * > groups;
    vector<QueryDeclaration> selectables(qo->getSelectablesAsQDs());
    for (vector<SuperClause *> &clauses: clauseGroups) {
        bool isEssential = true;

        if (isDupClauseRemovalEnabled) clauses = OptimizerUtils::removeDuplicates(clauses);
        if (isSimplifyGroupsEnabled) isEssential = OptimizerUtils::hasSynonymOverlap(selectables, clauses);

        if (isIntraGroupSortEnabled && isDynamicPollingEnabled) {
            groups.push_back(new PKBAwareGroup(clauses, adapter, isEssential));
        } else if (isIntraGroupSortEnabled) {
            groups.push_back(new SortedGroup(clauses, isEssential));
        } else {
            groups.push_back(new FifoGroup(clauses, isEssential));
        }
    }

    // step 3: Wrap clause groups under an ClauseGroup
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
         << "isSimplifyGroupsEnabled: " << isSimplifyGroupsEnabled << endl
         << "isIntraGroupSortEnabled: " << isIntraGroupSortEnabled << endl
         << "isInterGroupSortEnabled: " << isInterGroupSortEnabled << endl
         << "isDynamicPollingEnabled: " << isDynamicPollingEnabled << endl
         << "isDupClauseRemovalEnabled: " << isDupClauseRemovalEnabled << endl;
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

QueryOptimizer &QueryOptimizer::setSimplifyGroups(bool isOn) {
    isSimplifyGroupsEnabled = isOn;
    return *this;
}

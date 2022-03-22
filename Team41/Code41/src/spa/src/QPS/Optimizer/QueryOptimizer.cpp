#include "QueryOptimizer.h"

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject &qo) {
  const AbstractGroups& groups = isInterGroupSortEnabled
	  ? static_cast<const AbstractGroups&>(SortedGroups())
	  : static_cast<const AbstractGroups&>(FifoGroups());

  // step 1: Take query object, extract clauses, group them based on synonym dependencies
  vector<vector<SuperClause*>> clauseGroups;
  if (isClauseGroupingEnabled) {
	// clause grouping enabled - form dependency graph and split clauses
	for (SuperClause *cl : qo.getSuperClauses()) {
	  clauseDepGraph.registerClause(cl);
	}
	clauseGroups = clauseDepGraph.split();
  } else {
	// no clause grouping - just one group of all clauses
	clauseGroups.push_back(qo.getSuperClauses());
  }

  // step 2: Add clause groups
  for (auto &clauseGroup: clauseGroups) {

  }
//  clauseGroups.sortGroups();
//
//  // step 3: Sort each individual clause group
//  clauseGroups.sortEachGroup();

  return {adapter, qo, clauseGroups};
}

OptimizedQueryObject QueryOptimizer::getOptimizedQueryObject() {
  return optimizedQueryObject;
}

void QueryOptimizer::printPlan() {
  // TODO implement
  cout << "print plan not implemented" << endl;
}
ostream &operator<<(ostream &os, const QueryOptimizer &obj) {
	return os << "isClauseGroupingEnabled: " << obj.isClauseGroupingEnabled << endl
				<< "isIntraGroupSortEnabled: " << obj.isIntraGroupSortEnabled << endl
				<< "isInterGroupSortEnabled: " << obj.isInterGroupSortEnabled << endl
				<< "isDynamicPollingEnabled: " << obj.isDynamicPollingEnabled << endl;
}
QueryOptimizerBuilder QueryOptimizer::create() {
  return QueryOptimizerBuilder{};
}

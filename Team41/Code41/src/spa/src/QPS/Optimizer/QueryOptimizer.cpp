#include "QueryOptimizer.h"
#include "ClauseGroup.h"
#include "ClauseGroups.h"

QueryOptimizer::QueryOptimizer(PKBManager *pkbManager) : adapter(PKBAdapter(pkbManager)), clauseDepGraph(adapter) {
}

void QueryOptimizer::optimize(QueryObject &qo, bool isDynamic) {
  // step 1: Take query object, extract clauses, group them based on synonym dependencies
  ClauseGroups clauseGroups = divideClausesIntoGroups(qo);

  // step 2: Take clause groups, sort each groups by group characteristics
  clauseGroups.sortGroups();

  // step 3: Sort each individual clause group
  clauseGroups.sortEachGroup();

  optimizedQueryObject = {adapter, qo, clauseGroups, isDynamic};
}

ClauseGroups QueryOptimizer::divideClausesIntoGroups(QueryObject &qo) {
  for (SuperClause *cl : qo.getSuperClauses()) {
	clauseDepGraph.registerClause(cl);
  }
  return clauseDepGraph.split();
}

OptimizedQueryObject QueryOptimizer::getOptimizedQueryObject() {
  return optimizedQueryObject;
}

void QueryOptimizer::printPlan() {
  // TODO implement
  cout << "print plan not implemented" << endl;
}

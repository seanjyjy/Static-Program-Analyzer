#include "QueryOptimizer.h"
#include "ClauseGroup.h"
#include "ClauseGroups.h"

QueryOptimizer::QueryOptimizer() = default;

OptimizedQueryObject QueryOptimizer::optimize(QueryObject &qo) {
    // step 1: Take query object, extract clauses, group them based on synonym dependencies
    ClauseGroups clauseGroups = divideClausesIntoGroups(qo);

    // step 2: Take clause groups, sort each groups by group characteristics
    clauseGroups.sortGroups();

    // step 3: Sort each individual clause group
    clauseGroups.sortEachGroup();

    // TODO: for now, assume the original query object is not needed.
    return { clauseGroups };
}

ClauseGroups QueryOptimizer::divideClausesIntoGroups(QueryObject &qo) {
    for (const QueryClause& qc: qo.clauses) {
        clauseDepGraph.registerClause(qc);
    }
    for (const PatternClause& pc: qo.patternClauses) {
        clauseDepGraph.registerClause(pc);
    }
    return clauseDepGraph.split();
}

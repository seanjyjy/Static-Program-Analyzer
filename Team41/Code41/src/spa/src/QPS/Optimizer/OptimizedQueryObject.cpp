#include "OptimizedQueryObject.h"

OptimizedQueryObject::OptimizedQueryObject(): QueryObject({}, {}, {}, {}, false) {};

OptimizedQueryObject::OptimizedQueryObject(PKBAdapter &pkb, const QueryObject &qo, ClauseGroups &cg, bool isDynamic):
        QueryObject(qo), clauseGroups(cg), isDynamic(isDynamic), currSource(ClauseGroup(pkb)) {}

bool OptimizedQueryObject::hasNextClause() {
    if (!currSource.hasNextClause() && !clauseGroups.hasNextGroup()) return false;
    return true;
}

SuperClause* OptimizedQueryObject::nextClause() {
    // no more clauses in current clause group, no more clause groups -> throw error
    if (!hasNextClause()) throw runtime_error("no more clauses left to consume");
    if (!currSource.hasNextClause() && clauseGroups.hasNextGroup()) currSource = clauseGroups.getNextGroup();
    return isDynamic ? currSource.getNextClauseDynamic() : currSource.getNextClauseStatic();
}

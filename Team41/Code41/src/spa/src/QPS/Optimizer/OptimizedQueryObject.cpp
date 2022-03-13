#include "OptimizedQueryObject.h"

OptimizedQueryObject::OptimizedQueryObject(PKBAdapter &pkb, QueryObject &qo, ClauseGroups &cg, bool isDynamic):
queryObject(qo), clauseGroups(cg), isDynamic(isDynamic), currSource(ClauseGroup(pkb)) {}

bool OptimizedQueryObject::isQueryValid() {
    // TODO stub
    return false;
}

bool OptimizedQueryObject::isSelectingBoolean() {
    // TODO stub
    return false;
}

vector<ClauseVariable> OptimizedQueryObject::getSelectSynonyms() {
    // TODO stub
    return {};
}

bool OptimizedQueryObject::hasNextClause() {
    if (!currSource.hasNextClause() && !clauseGroups.hasNextGroup()) return false;
    return true;
}

TempClause OptimizedQueryObject::nextClause() {
    // no more clauses in current clause group, no more clause groups -> throw error
    if (!hasNextClause()) throw runtime_error("no more clauses left to consume");
    if (!currSource.hasNextClause() && clauseGroups.hasNextGroup()) currSource = clauseGroups.getNextGroup();
    return isDynamic ? currSource.getNextClauseDynamic() : currSource.getNextClauseStatic();
}


#include "OptimizedQueryObject.h"

OptimizedQueryObject::OptimizedQueryObject() : QueryObject({}, {}, {}, {}, false) {}

OptimizedQueryObject::OptimizedQueryObject(QueryObject *qo, AbstractGroups *cg) :
        QueryObject(*qo), clauseGroups(cg) {}

bool OptimizedQueryObject::empty() {
    return clauseGroups->empty();
}

SuperClause *OptimizedQueryObject::popClause() {
    if (empty()) throw runtime_error("OptimizedQueryObject: no more clauses left to consume");
    return clauseGroups->pop();
}

void OptimizedQueryObject::printPlan() {
    clauseGroups->print();
}

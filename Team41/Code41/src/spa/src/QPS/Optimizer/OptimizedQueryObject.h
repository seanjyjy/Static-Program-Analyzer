#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"
#include "AbstractGroups.h"

class OptimizedQueryObject : public QueryObject {
private:
    AbstractGroups *clauseGroups;

public:
    OptimizedQueryObject();

    OptimizedQueryObject(QueryObject *qo, AbstractGroups *cg);

    SuperClause *popClause();

    bool empty();

    void printPlan();
};

#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"
#include "ClauseGroups.h"

class OptimizedQueryObject : public QueryObject {
private:
    ClauseGroups *clauseGroups;

public:
    OptimizedQueryObject();

    OptimizedQueryObject(QueryObject *qo); // only used when query object has an error - skip optimization

    OptimizedQueryObject(QueryObject *qo, ClauseGroups *cg);

    SuperClause *popClause() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    bool empty() override;

    void print() override;
};

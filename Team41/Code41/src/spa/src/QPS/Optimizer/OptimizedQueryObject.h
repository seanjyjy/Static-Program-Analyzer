#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"
#include "ClauseGroups.h"

/**
 * Represents a Optimized QueryObject (OQO), with additional data structures to support optimization.
 */
class OptimizedQueryObject : public QueryObject {
private:
    // the groups of clause groups containing all clauses given in the PQL query.
    ClauseGroups *clauseGroups;

public:
    /**
     * Creates an empty Optimized Query Object, with an empty underlying Query Object.
     */
    OptimizedQueryObject();

    /**
     * Creates an Optimized Query Object with no clauses to consume.
     * Mainly used when there was an error creating the OQO but there is a need to access the underlying query object.
     */
    OptimizedQueryObject(QueryObject *qo); // only used when query object has an error - skip optimization

    /**
     * Creates an Optimized Query Object with given clause groups to consume.
     */
    OptimizedQueryObject(QueryObject *qo, ClauseGroups *cg);

    ~OptimizedQueryObject();

    SuperClause *popClause() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    bool empty() override;

    void print() override;
};

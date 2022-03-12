#pragma once

#include "ClauseGroups.h"
#include "QPS/QueryObject.h"
#include "QPS/Optimizer/TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"

class OptimizedQueryObject {
private:
    QueryObject queryObject;
    ClauseGroups clauseGroups;
    ClauseGroup currSource;

    // for dynamic optimization
    bool isDynamic = false;
public:
    OptimizedQueryObject(PKBAdapter &pkb, QueryObject &qo, ClauseGroups &cg, bool isDynamic=false);

    TempClause nextClause();
    bool hasNextClause();
    bool isQueryValid();
    bool isSelectingBoolean();
    vector<ClauseVariable> getSelectSynonyms();
};

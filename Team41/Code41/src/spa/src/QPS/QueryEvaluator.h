#pragma once

#include <unordered_set>
#include <string>
#include <vector>

#include "PKB/PKB.h"
#include "QPS/Evaluator/Evaluator.h"
#include "QPS/Evaluator/UsesSEvaluator.h"
#include "QPS/Evaluator/UsesPEvaluator.h"
#include "QPS/Evaluator/ModifiesSEvaluator.h"
#include "QPS/Evaluator/ModifiesPEvaluator.h"
#include "QPS/Evaluator/SelectSynonymEvaluator.h"
#include "QPS/Evaluator/FollowsEvaluator.h"
#include "QPS/Evaluator/FollowsTEvaluator.h"
#include "QPS/Evaluator/ParentEvaluator.h"
#include "QPS/Evaluator/ParentTEvaluator.h"
#include "QueryObject.h"
#include "QueryClause.h"

class QueryEvaluator {
private:
    PKB* pkb;
public:
    explicit QueryEvaluator(PKB* pkb);
    std::unordered_set<std::string> evaluateQuery(QueryObject *queryObject);
    Table* evaluate(QueryClause& clause);
};

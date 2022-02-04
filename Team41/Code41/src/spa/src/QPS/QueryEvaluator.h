#pragma once

#include <set>
#include <string>
#include <vector>
#include "QPS/Evaluator/Evaluator.h"
#include "QueryObject.h"

class QueryEvaluator {
private:
    PKB* pkb;
public:
    explicit QueryEvaluator(PKB* pkb);
    std::set<std::string> evaluateQuery(QueryObject *queryObject);
    Table* evaluate(string clause, PKB* pkb);
};
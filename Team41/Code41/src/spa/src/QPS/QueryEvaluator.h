#pragma once

#include <set>
#include <string>
#include <vector>
#include "QPS/Evaluator.h"
#include "QueryObject.h"

class QueryEvaluator {
    std::set<std::string> evaluateQuery(QueryObject *queryObject);
    Evaluator* getEvaluator(const std::string& clause);
};

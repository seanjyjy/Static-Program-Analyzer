#pragma once

#include <unordered_set>
#include <string>
#include <vector>

#include "PKB/PKBClient.h"
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
#include "QPS/Evaluator/CallsEvaluator.h"
#include "QPS/Evaluator/CallsTEvaluator.h"
#include "QPS/Evaluator/AssignPatternEvaluator.h"
#include "QPS/Evaluator/IfPatternEvaluator.h"
#include "QPS/Evaluator/WhilePatternEvaluator.h"
#include "QueryObject.h"
#include "QueryClause.h"

class QueryEvaluator {
private:
    PKBClient* pkb;

    /**
     * Deletes a table after it is not being used.
     *
     * @param table Table to be deleted.
     */
    void safeDeleteTable(Table* table);
public:
    explicit QueryEvaluator(PKBClient* pkb);

    /**
     * Based on the QueryObject, evaluates the query provided return a set of results.
     *
     * @param queryObject QueryObject.
     * @return A set of results based on the query provided.
     */
    std::unordered_set<std::string> evaluateQuery(QueryObject *queryObject);

    /**
     * Constructs a table based on the QueryClause provided.
     *
     * @param clause QueryClause.
     * @return A table that contains information based on the QueryClause provided.
     */
    Table* evaluate(QueryClause& clause);

    /**
     * Constructs a table based on the PatternClause provided.
     *
     * @param clause PatternClause.
     * @return A table that contains information based on the PatternClause provided.
     */
    Table* evaluate(PatternClause& clause);
};

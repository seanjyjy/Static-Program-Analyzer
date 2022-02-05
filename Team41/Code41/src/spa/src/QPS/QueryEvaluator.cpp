#include <stdexcept>
#include "QueryEvaluator.h"
#include "QPS/Evaluator.h"

std::set<std::string> QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    std::set<std::string> result;
    std::set<std::string> emptyResult;

    PQLTable* resultTable = nullptr;

    if (!queryObject->isQueryValid) {
        return result;
    }

    for (const auto& clause : queryObject->clauses) {
        Evaluator* evaluator = getEvaluator(clause.getLeftClauseVariable().getLabel()); // todo: change placeholder param
        PQLTable* intermediateTable = evaluator->evaluate();

        if (intermediateTable->isEmpty()) {
            return emptyResult;
        }

        // The original table is not mutated but rather a new table is created
        PQLTable* temp = resultTable;
        resultTable = resultTable->mergeJoin(intermediateTable);
        delete temp;

        if (resultTable->isEmpty()) {
            return emptyResult;
        }
    }

    // copy result from resultTable to result here when can think of based on queryObject schema
    // auto rows = resultTable->getRows();

    return result;
}

// can be change into clause.type or something!
Evaluator *QueryEvaluator::getEvaluator(const std::string& clause) {
       if (clause == "FOLLOWS") {
           return nullptr;
       } else if (clause == "FOLLOWS_T") {
           return nullptr;
       } else if (clause == "PARENT") {
           return nullptr;
       } else if (clause == "PARENT_T") {
           return nullptr;
       } else if (clause == "USES_S") {
           return nullptr;
       } else if (clause == "USES_P") {
           return nullptr;
       } else if (clause == "MODIFIES_S") {
           return nullptr;
       } else if (clause == "MODIFIES_P") {
           return nullptr;
       } else if (clause == "PATTERN") {
           return nullptr;
       } else {
           throw std::runtime_error("unknown clause of type " + clause);
       }
}
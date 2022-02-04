#include <stdexcept>
#include "QueryEvaluator.h"
#include "QPS/Evaluator/Evaluator.h"

QueryEvaluator::QueryEvaluator(PKB *pkb) { this->pkb = pkb; }

std::set<std::string> QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    std::set<std::string> result;
    std::set<std::string> emptyResult;

    Table* resultTable = nullptr;

    if (!queryObject->isQueryValid) {
        return emptyResult;
    }

    for (const auto& clause : queryObject->clauses) {
        // Ignore warning for now
        Table* intermediateTable = this->evaluate(clause, this->pkb); // false table is return it should be empty?

        if (intermediateTable->isEmpty()) {
            return emptyResult;
        }

        // The original table is not mutated but rather a new table is created
        Table* temp = resultTable;
        resultTable = resultTable->mergeJoin(intermediateTable);
        // TODO figuure out destructor
        delete temp;

        if (resultTable->isEmpty()) {
            return emptyResult;
        }
    }

    // copy result from resultTable to result here when can think of based on queryObject schema
    // auto rows = resultTable->getRows();

    return result;
}

Table *QueryEvaluator::evaluate(string clause, PKB *pkb) {
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



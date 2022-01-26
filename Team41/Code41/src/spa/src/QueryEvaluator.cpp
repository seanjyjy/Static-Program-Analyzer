//
// Created by lum jian yang sean on 26/1/22.
//
#include <stdexcept>
#include "QueryEvaluator.h"
#include "Evaluator.h"

/*
 Refer to stub in QueryEvaluator.h (not real or chosen design just temp)

 Assuming QueryProject provides
 1. A field that determines if this is a synthetically and semantically correct query / or ideally a way to know if the current query is ok
 2. A field contains all the declarations
 3. A field that contains all the clauses
    3.1 perhaps clauses can be an object that provides the type? // some ENUM?
    3.2 and also provide lhs and rhs variable? (should apply to all clauses right)
 */
std::set<std::string> QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    std::set<std::string> result;
    // not sure if it is better to create this or empty from result
    std::set<std::string> emptyResult;

    PQLTable* resultTable = nullptr;

    if (!queryObject->isQueryValid) {
        return result;
    }

    for (const auto& clause : queryObject->clauses) {
        Evaluator* evaluator = getEvaluator(clause);
        PQLTable* intermediateTable = evaluator->evaluate();

        if (intermediateTable->isEmpty()) {
            return emptyResult;
        }

        resultTable->intersect(intermediateTable);

        if (resultTable->isEmpty()) {
            return emptyResult;
        }
    }

    // copy result from resultTable to result here

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
}
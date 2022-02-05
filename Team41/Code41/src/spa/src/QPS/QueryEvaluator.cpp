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

    for (auto& clause : queryObject->clauses) {
        // Ignore warning for now
        Table* intermediateTable = this->evaluate(clause); // false table is return it should be empty?

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

Table *QueryEvaluator::evaluate(QueryClause& clause) {
    switch(clause.type) {
        case QueryClause::clause_type::follows:
            return nullptr;
        case QueryClause::clause_type::followsT:
            return nullptr;
        case QueryClause::clause_type::parent:
            return nullptr;
        case QueryClause::clause_type::parentT:
            return nullptr;
        case QueryClause::clause_type::usesS:
            return UsesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::usesP:
            return UsesPEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesS:
            return ModifiesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesP:
            return ModifiesPEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::pattern:
            return nullptr;
        default:
            throw std::runtime_error("unknown clause of type " + to_string(clause.type));
    }
}



#include <stdexcept>
#include "QueryEvaluator.h"
#include "QPS/Evaluator/Evaluator.h"

QueryEvaluator::QueryEvaluator(PKB *pkb) { this->pkb = pkb; }

std::unordered_set<std::string> QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    std::unordered_set<std::string> emptyResult;

    if (!queryObject->isQueryValid) {
        return emptyResult;
    }

    auto selectSynonym = queryObject->selectSynonym;
    Table *resultTable;

    // TODO: Change it to truth table to speed up initial config
    try {
        resultTable = SelectSynonymEvaluator::evaluate(selectSynonym, this->pkb);
    } catch (const runtime_error& error) {
        resultTable = FalseTable::getTable();
    }

    if (resultTable->isEmpty()) {
        return emptyResult;
    }
    printf("test first element lerft variable: %d\n", queryObject->clauses[0].getLeftClauseVariable().type);
    for (auto clause : queryObject->clauses) {
        try {
            Table* intermediateTable = this->evaluate(clause);
            if (intermediateTable->isEmpty()) {
                return emptyResult;
            }

            Table* temp = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);
            // TODO figure out destructor
            delete temp;

        } catch (const runtime_error& error) {
            resultTable = FalseTable::getTable();
        }

        if (resultTable->isEmpty()) {
            return emptyResult;
        }
    }

    // copy result from resultTable to result here when can think of based on queryObject schema
     unordered_set<string> result = resultTable->getColumn(selectSynonym.synonym);
     delete resultTable;

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



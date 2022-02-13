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
    Table *resultTable = nullptr;

    auto queryClauses = queryObject->clauses;
    auto patternClauses = queryObject->patternClauses;
    auto suchThatClPtr = queryClauses.begin();
    auto patternClPtr = patternClauses.begin();
    try {
        while (suchThatClPtr != queryClauses.end() || patternClPtr != patternClauses.end()) {
            Table *intermediateTable;
            if (suchThatClPtr != queryClauses.end()) {
                intermediateTable = this->evaluate(*suchThatClPtr);
                suchThatClPtr++;
            } else {
                intermediateTable = this->evaluate(*patternClPtr);
                patternClPtr++;
            }

            if (intermediateTable->isEmpty()) {
                return emptyResult;
            }

            if (resultTable == nullptr) {
                resultTable = intermediateTable;
                continue;
            }

            Table* temp = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);
            // TODO figure out destructor
            delete temp;


            if (resultTable->isEmpty()) {
                return emptyResult;
            }
        }

        for (const auto& declaration : queryObject->declarations) {
            Table* intermediateTable = SelectSynonymEvaluator::evaluate(declaration, this->pkb);

            if (intermediateTable->isEmpty()) {
                return emptyResult;
            }

            if (resultTable == nullptr) {
                resultTable = intermediateTable;
                continue;
            }

            resultTable = resultTable->mergeJoin(intermediateTable);

            if (resultTable->isEmpty()) {
                return emptyResult;
            }
        }
    } catch (const runtime_error& error) {
        return emptyResult;
    }

    // copy result from resultTable to result here when can think of based on queryObject schema
    unordered_set<string> result = resultTable->getColumn(selectSynonym.synonym);
    delete resultTable;

    return result;
}

Table *QueryEvaluator::evaluate(QueryClause &clause) {
    switch (clause.type) {
        case QueryClause::clause_type::follows:
            return FollowsEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::followsT:
            return FollowsTEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::parent:
            return ParentEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::parentT:
            return ParentEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::usesS:
            return UsesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::usesP:
            return UsesPEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesS:
            return ModifiesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesP:
            return ModifiesPEvaluator::evaluate(clause, this->pkb);
        default:
            throw std::runtime_error("unknown clause of type " + to_string(clause.type));
    }
}

Table *QueryEvaluator::evaluate(PatternClause &clause) {
    auto type = clause.getSynonym().type;
    switch (type) {
        case QueryDeclaration::design_entity_type::ASSIGN:
            return PatternEvaluator::evaluate(clause, this->pkb);
        default:
            throw std::runtime_error("unknown pattern clause of type " + to_string(type));
    }
}

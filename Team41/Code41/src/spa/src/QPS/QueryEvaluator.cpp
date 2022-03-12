#include "QueryEvaluator.h"
#include "QPS/Evaluator/Evaluator.h"

QueryEvaluator::QueryEvaluator(PKBClient *pkb) { this->pkb = pkb; }

unordered_set<string> QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    unordered_set<string> emptyResult;
    unordered_set<string> result;

    if (!queryObject->isQueryValid) {
        return emptyResult;
    }

    Table *resultTable = new TrueTable();
    auto selectSynonym = queryObject->selectSynonym;
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
                safeDeleteTable(intermediateTable);
                return buildResult(queryObject, new FalseTable());
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                return buildResult(queryObject, new FalseTable());
            }
        }

        for (const auto &declaration: queryObject->declarations) {
            Table *intermediateTable = SelectSynonymEvaluator::evaluate(declaration, this->pkb);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                return buildResult(queryObject, new FalseTable());
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                return buildResult(queryObject, new FalseTable());
            }
        }
    } catch (SemanticException& error) {
        std::cout << error.what() << std::endl;

        return buildResult(queryObject, new FalseTable());
    } catch (const runtime_error& error) {
        std::cout << error.what() << std::endl;

        return buildResult(queryObject, new FalseTable());
    }

    return buildResult(queryObject, resultTable);
}

unordered_set<string> QueryEvaluator::buildResult(QueryObject *queryObject, Table *resultTable) {
    unordered_set<string> result;

    if (queryObject->isSelectingBoolean()) {
        // Occurs when result table is a false table or pql table which is empty
        if (resultTable->isEmpty()) {
            result.insert("False");
        } else {
            result.insert("True");
        }
    } else {
        vector<string> synonyms = getSynonyms(queryObject);

        // copy result from resultTable to result here when can think of based on queryObject schema
        result = resultTable->getColumns(synonyms);
    }
    safeDeleteTable(resultTable);
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
            return ParentTEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::usesS:
            return UsesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::usesP:
            return UsesPEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesS:
            return ModifiesSEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::modifiesP:
            return ModifiesPEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::calls:
            return CallsEvaluator::evaluate(clause, this->pkb);
        case QueryClause::clause_type::callsT:
            return CallsTEvaluator::evaluate(clause, this->pkb);
        default:
            throw std::runtime_error("unknown clause of type " + to_string(clause.type));
    }
}

Table *QueryEvaluator::evaluate(PatternClause &clause) {
    auto type = clause.getSynonym().type;
    switch (type) {
        case QueryDeclaration::design_entity_type::ASSIGN:
            return AssignPatternEvaluator::evaluate(clause, this->pkb);
        case QueryDeclaration::design_entity_type::IF:
            return IfPatternEvaluator::evaluate(clause, this->pkb);
        case QueryDeclaration::design_entity_type::WHILE:
            return WhilePatternEvaluator::evaluate(clause, this->pkb);
        default:
            throw std::runtime_error("unknown pattern clause of type " + to_string(type));
    }
}

void QueryEvaluator::safeDeleteTable(Table* tableToDelete, Table* resultTable) {
    if (tableToDelete == resultTable) {
        return;
    }

    if (tableToDelete != nullptr) {
        delete tableToDelete;
    }
}

void QueryEvaluator::safeDeleteTable(Table *tableToDelete) {
    safeDeleteTable(tableToDelete, nullptr);
}

vector<string> QueryEvaluator::getSynonyms(QueryObject *queryObject) {
    unordered_set<string> seenSynonyms;
    vector<string> synonyms;

    // TDOO: change to the list of clauses from the Optimized in the future
    for (auto& queryDeclaration : queryObject->selectSynonyms) {
        auto synonym = queryDeclaration.synonym;
        if (seenSynonyms.find(synonym) == seenSynonyms.end()) {
            seenSynonyms.insert(synonym);
            synonyms.insert(synonym);
        }
    }

    return synonyms;
}

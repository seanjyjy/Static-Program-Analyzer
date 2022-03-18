#include "QueryEvaluator.h"
#include "QPS/Evaluator/Evaluator.h"

QueryEvaluator::QueryEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
    this->nextKBAdapter = new NextKBAdapter(pkb);
}

QueryResult QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    unordered_set<string> emptyResult;
    unordered_set<string> result;

    if (!queryObject->isQueryValid) {
        return {queryObject->selectTarget, nullptr, pkb, false};
    }

    if (!EvaluatorUtils::validateDeclarations(queryObject->declarations) ||
        !EvaluatorUtils::AttrUtils::validateSelectTarget(&queryObject->selectTarget)) {
        return {queryObject->selectTarget, new FalseTable(), pkb, true};
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
                safeDeleteTable(resultTable);
                return {queryObject->selectTarget, new FalseTable(), pkb, true};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->selectTarget, new FalseTable(), pkb, true};
            }
        }

        for (const auto &declaration: queryObject->declarations) {
            Table *intermediateTable = SelectSynonymEvaluator(this->pkb).evaluate(declaration);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                safeDeleteTable(resultTable);
                return {queryObject->selectTarget, new FalseTable(), pkb, true};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->selectTarget, new FalseTable(), pkb, true};
            }
        }
    } catch (SemanticException& error) {
        std::cout << error.what() << std::endl;
        delete resultTable;
        return {queryObject->selectTarget, new FalseTable(), pkb, true};
    } catch (const runtime_error& error) {
        std::cout << error.what() << std::endl;
        delete resultTable;
        return {queryObject->selectTarget, new FalseTable(), pkb, true};
    }

    return {queryObject->selectTarget, resultTable, pkb, true};
}

Table *QueryEvaluator::evaluate(QueryClause &clause) {
    switch (clause.type) {
        case QueryClause::clause_type::follows:
            return FollowsEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::followsT:
            return FollowsTEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::parent:
            return ParentEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::parentT:
            return ParentTEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::usesS:
            return UsesSEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::usesP:
            return UsesPEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::modifiesS:
            return ModifiesSEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::modifiesP:
            return ModifiesPEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::calls:
            return CallsEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::callsT:
            return CallsTEvaluator(pkb).evaluate(clause);
        case QueryClause::clause_type::next:
            return NextEvaluator(pkb, nextKBAdapter).evaluate(clause);
        case QueryClause::clause_type::nextT: // NextT affects affectsT should take in an extra cache when it is supported
            return NextTEvaluator(pkb, nextKBAdapter).evaluate(clause);
        case QueryClause::clause_type::affects:
            return nullptr;
        case QueryClause::clause_type::affectsT:
            return nullptr;
        default:
            throw std::runtime_error("unknown clause of type " + to_string(clause.type));
    }
}

Table *QueryEvaluator::evaluate(PatternClause &clause) {
    auto type = clause.getSynonym().type;
    switch (type) {
        case QueryDeclaration::design_entity_type::ASSIGN:
            return AssignPatternEvaluator(pkb).evaluate(clause);
        case QueryDeclaration::design_entity_type::IF:
            return IfPatternEvaluator(pkb).evaluate(clause);
        case QueryDeclaration::design_entity_type::WHILE:
            return WhilePatternEvaluator(pkb).evaluate(clause);
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

QueryEvaluator::~QueryEvaluator() {
    // TODO: check if this cause an error as this also takes in PKB!
    delete nextKBAdapter;
}


void QueryEvaluator::safeDeleteTable(Table *tableToDelete) {
    safeDeleteTable(tableToDelete, nullptr);
}

vector<string> QueryEvaluator::getSynonyms(QueryObject *queryObject) {
    vector<string> synonyms;

    for (auto& selectable : queryObject->getSelectables()) {
        QueryDeclaration queryDeclaration = selectable.getSynonym();
        string label = queryDeclaration.synonym;

        if (selectable.getType() == Selectable::ATTR_REF) {
            label.append(".").append(to_string(selectable.getAttr()));
        }

        synonyms.push_back(label);
    }

    return synonyms;
}

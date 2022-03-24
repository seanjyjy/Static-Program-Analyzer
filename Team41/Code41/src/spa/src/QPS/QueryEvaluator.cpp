#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
    this->nextKBAdapter = new NextKBAdapter(pkb);
    this->affectsKBAdapter = new AffectsKBAdapter(pkb);
}

QueryResult QueryEvaluator::evaluateQuery(OptimizedQueryObject *queryObject) {
    unordered_set<string> emptyResult;
    unordered_set<string> result;

    if (!queryObject->isValid()) {
        return {queryObject->getSelectTarget(), nullptr, false};
    }

    if (!EvaluatorUtils::validateDeclarations(queryObject->getDeclarations()) ||
        !EvaluatorUtils::AttrUtils::validateSelectTarget(&queryObject->getSelectTarget())) {
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    Table *resultTable = new TrueTable();

    try {
        while (!queryObject->empty()) {
            SuperClause *clause = queryObject->popClause();
            Table *intermediateTable = this->evaluate(clause);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);
            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }
        }

        for (const auto &declaration: queryObject->getDeclarations()) {
            Header header = resultTable->getHeader();
            if (header.find(declaration.synonym) != header.end()) {
                continue;
            }

            Table *intermediateTable = SelectSynonymEvaluator(this->pkb).evaluate(declaration);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }
        }
    } catch (SemanticException &error) {
        cout << error.what() << endl;
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    } catch (const runtime_error &error) {
        cout << error.what() << endl;
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    return {queryObject->getSelectTarget(), resultTable};
}

Table *QueryEvaluator::evaluate(SuperClause *clause) {
    if (clause->isPatternClause()) {
        return evaluate(clause->getPatternClause());
    } else if (clause->isWithClause()) {
        return evaluate(clause->getWithClause());
    }

    return evaluate(clause->getSuchThatClause());
}

Table *QueryEvaluator::evaluate(const QueryClause &clause) {
    switch (clause.getType()) {
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
        case QueryClause::clause_type::nextT:
            return NextTEvaluator(pkb, nextKBAdapter).evaluate(clause);
        case QueryClause::clause_type::affects:
            return AffectsEvaluator(pkb, affectsKBAdapter).evaluate(clause);
        case QueryClause::clause_type::affectsT:
            return AffectsTEvaluator(pkb, affectsKBAdapter).evaluate(clause);
        default:
            throw std::runtime_error("unknown clause of type " + to_string(clause.type));
    }
}

Table *QueryEvaluator::evaluate(const PatternClause &clause) {
    auto type = clause.getSynonym().getType();
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

Table *QueryEvaluator::evaluate(const WithClause &clause) {
    return WithEvaluator(pkb).evaluate(clause);
}

void QueryEvaluator::safeDeleteTable(Table *tableToDelete, Table *resultTable) {
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
    delete affectsKBAdapter;
}


void QueryEvaluator::safeDeleteTable(Table *tableToDelete) {
    safeDeleteTable(tableToDelete, nullptr);
}

vector<string> QueryEvaluator::getSynonyms(QueryObject *queryObject) {
    vector<string> synonyms;

    for (auto &selectable: queryObject->getSelectables()) {
        QueryDeclaration queryDeclaration = selectable.getSynonym();
        string label = queryDeclaration.synonym;

        if (selectable.getType() == Selectable::ATTR_REF) {
            label.append(".").append(to_string(selectable.getAttr()));
        }

        synonyms.push_back(label);
    }

    return synonyms;
}

// TODO: DELETE WHEN ALL IS WELL
QueryResult QueryEvaluator::evaluateQuery(QueryObject *queryObject) {
    unordered_set<string> emptyResult;
    unordered_set<string> result;

    if (!queryObject->isValid()) {
        return {queryObject->getSelectTarget(), nullptr, false};
    }

    if (!EvaluatorUtils::validateDeclarations(queryObject->getDeclarations()) ||
        !EvaluatorUtils::AttrUtils::validateSelectTarget(&queryObject->getSelectTarget())) {
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    Table *resultTable = new TrueTable();

    try {
        for (auto clause : queryObject->getSuperClauses()) {
            Table* intermediateTable = this->evaluate(clause);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);
            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }
        }

        for (const auto &declaration: queryObject->getDeclarations()) {
            Header header = resultTable->getHeader();
            if (header.find(declaration.synonym) != header.end()) {
                continue;
            }

            Table *intermediateTable = SelectSynonymEvaluator(this->pkb).evaluate(declaration);

            if (intermediateTable->isEmpty()) {
                safeDeleteTable(intermediateTable);
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }

            Table *ogTable = resultTable;
            resultTable = resultTable->mergeJoin(intermediateTable);

            safeDeleteTable(ogTable, resultTable);
            safeDeleteTable(intermediateTable, resultTable);

            if (resultTable->isEmpty()) {
                safeDeleteTable(resultTable);
                return {queryObject->getSelectTarget(), new FalseTable()};
            }
        }
    } catch (SemanticException& error) {
        cout << error.what() << endl;
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    } catch (const runtime_error& error) {
        cout << error.what() << endl;
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    return {queryObject->getSelectTarget(), resultTable};
}

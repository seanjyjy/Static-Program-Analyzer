#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
    this->nextKBAdapter = new NextKBAdapter(pkb);
    this->affectsKBAdapter = new AffectsKBAdapter(pkb);
}

Table *QueryEvaluator::mergeTable(Table *resultTable, Table *intermediateTable) {
    if (intermediateTable->isEmpty()) {
        safeDeleteTable(intermediateTable);
        safeDeleteTable(resultTable);
        return new FalseTable();
    }

    Table *ogTable = resultTable;
    resultTable = resultTable->mergeJoin(intermediateTable);

    safeDeleteTable(ogTable, resultTable);
    safeDeleteTable(intermediateTable, resultTable);

    if (resultTable->isEmpty()) {
        safeDeleteTable(resultTable);
        return new FalseTable();
    }

    return resultTable;
}

Table *QueryEvaluator::evaluateClauses(Table *resultTable, OptimizedQueryObject *queryObject) {
    while (!queryObject->empty()) {
        SuperClause *clause = queryObject->popClause();
        Table *intermediateTable = this->evaluate(clause);
        Table *mergedTable = mergeTable(resultTable, intermediateTable);
        if (mergedTable->isEmpty()) {
            return mergedTable;
        }

        resultTable = mergedTable;
    }

    return resultTable;
}

Table *QueryEvaluator::evaluateSelectables(Table *resultTable, OptimizedQueryObject *queryObject) {
    for (const auto &selected: queryObject->getSelectables()) {
        Header header = resultTable->getHeader();
        if (header.find(selected.getSynonym().getSynonym()) != header.end())
            continue;

        Table *intermediateTable = SelectSynonymEvaluator(this->pkb).evaluate(selected.getSynonym());
        Table *mergedTable = mergeTable(resultTable, intermediateTable);
        if (mergedTable->isEmpty()) {
            return mergedTable;
        }

        resultTable = mergedTable;
    }

    return resultTable;
}

QueryResult QueryEvaluator::evaluateQuery(OptimizedQueryObject *queryObject) {
    if (!queryObject->isValid()) {
        return {queryObject->getSelectTarget(), nullptr, false};
    }

    if (queryObject->hasUseOfUndeclaredVariable() ||
        !EvaluatorUtils::validateDeclarations(queryObject->getDeclarations()) ||
        !EvaluatorUtils::AttrUtils::validateSelectTarget(&queryObject->getSelectTarget())) {
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    Table *resultTable = new TrueTable();

    try {
        Table *clauseResult = evaluateClauses(resultTable, queryObject);
        if (clauseResult->isEmpty()) {
            return {queryObject->getSelectTarget(), clauseResult};
        }

        Table *selectableResult = evaluateSelectables(clauseResult, queryObject);
        return {queryObject->getSelectTarget(), selectableResult};
    } catch (SemanticException &) {
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    } catch (const runtime_error &) {
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    }
}

Table *QueryEvaluator::evaluate(SuperClause *clause) {
    if (clause->isPatternClause()) {
        return evaluate(clause->getPatternClause());
    }

    if (clause->isWithClause()) {
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
            throw runtime_error("unknown clause of type " + to_string(clause.type));
    }
}

Table *QueryEvaluator::evaluate(const PatternClause &clause) {
    auto type = clause.getSynonym().getType();
    if (type->isAssign()) {
        return AssignPatternEvaluator(pkb).evaluate(clause);
    } else if (type->isIf()) {
        return IfPatternEvaluator(pkb).evaluate(clause);
    } else if (type->isWhile()) {
        return WhilePatternEvaluator(pkb).evaluate(clause);
    } else {
        throw runtime_error("unknown pattern clause type");
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
        tableToDelete = nullptr;
    }
}

QueryEvaluator::~QueryEvaluator() {
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
        for (auto clause: queryObject->getSuperClauses()) {
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
    } catch (SemanticException &) {
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    } catch (const runtime_error &) {
        safeDeleteTable(resultTable);
        return {queryObject->getSelectTarget(), new FalseTable()};
    }

    return {queryObject->getSelectTarget(), resultTable};
}

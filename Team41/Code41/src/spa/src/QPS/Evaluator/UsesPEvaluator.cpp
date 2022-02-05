#include "UsesPEvaluator.h"

Table* UsesPEvaluator::evaluate(QueryClause clause, PKB* pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (leftVariable.isIdentifier() && rightVariable.isIdentifer()) {
        return evaluateIdentifierIdentifier(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isSynonym()) {
        return evaluateIdentifierSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isWildCard()) {
        return evaluateIdentifierWildCard(pkb, leftVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isIdentifer()) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isSynonym()) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isWildCard()) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    return new FalseTable();
}

Table* UsesPEvaluator::evaluateIdentifierIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    bool isUsesP = pkb->isUsesP(left->getIdentifier(), right->getIdentifier());

    if (isUsesP) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* UsesPEvaluator::evaluateIdentifierSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left->getIdentifier());

    string column = right->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateIdentifierWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left->getIdentifier());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* UsesPEvaluator::evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfProcedures = pkb->getUsesPByVar(right->getIdentifier());

    string column = left->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfProcedureToVariable =  pkb->getAllUsesP();

    string firstColumn = left->getSynonym();
    string secondColumn = right->getSynonym();
    Header header = Header({firstColumn, secondColumn});
    Table* result = new PQLTable(header);

    for (auto& procedureToVariable : listOfProcedureToVariable) {
        Row* row = new Row();
        row->addEntry(firstColumn, procedureToVariable.first);
        row->addEntry(secondColumn, procedureToVariable.second);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateSynonymWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfProcedures =  pkb->getAllProcsUsingSomeVar();

    string column = left->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}

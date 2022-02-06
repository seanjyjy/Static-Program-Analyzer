#include "UsesPEvaluator.h"

Table* UsesPEvaluator::evaluate(QueryClause clause, PKB* pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (leftVariable.isIdentifier() && rightVariable.isIdentifier()) {
        return evaluateIdentifierIdentifier(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isSynonym()) {
        return evaluateIdentifierSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isWildCard()) {
        return evaluateIdentifierWildCard(pkb, leftVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isIdentifier()) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isSynonym()) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isWildCard()) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    return FalseTable::getTable();
}

Table* UsesPEvaluator::evaluateIdentifierIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    bool isUsesP = pkb->isUsesP(left.getLabel(), right.getLabel());

    if (isUsesP) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table* UsesPEvaluator::evaluateIdentifierSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateIdentifierWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left.getLabel());

    if (setOfVariables.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* UsesPEvaluator::evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfProcedures = pkb->getUsesPByVar(right.getLabel());

    string column = left.getLabel();
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

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
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

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}

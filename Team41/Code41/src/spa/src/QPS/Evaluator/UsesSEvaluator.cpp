#include "UsesSEvaluator.h"

Table* UsesSEvaluator::evaluate(QueryClause clause, PKBClient *pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::SUtils::isIntegerIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIntegerIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidIntegerSynonym(&leftVariable, &rightVariable)) {
        return evaluateIntegerSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isIntegerWildCard(&leftVariable, &rightVariable)) {
        return evaluateIntegerWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    throw SemanticException("Invalid query provided for Uses");
}

Table* UsesSEvaluator::evaluateIntegerIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    bool isUsesS = pkb->isUsesS(left.getLabel(), right.getLabel());

    if (isUsesS) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* UsesSEvaluator::evaluateIntegerSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getUsesByStmt(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesSEvaluator::evaluateIntegerWildCard(PKBClient* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getUsesByStmt(left.getLabel());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* UsesSEvaluator::evaluateSynonymIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStatements =  pkb->getUsesSByVar(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* UsesSEvaluator::evaluateSynonymSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStatementToVariable =  pkb->getAllUsesS();

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* result = new PQLTable(header);

    for (auto& statementToVariable : listOfStatementToVariable) {
        Row* row = new Row();
        row->addEntry(firstColumn, statementToVariable.first);
        row->addEntry(secondColumn, statementToVariable.second);
        result->addRow(row);
    }

    return result;
}

Table* UsesSEvaluator::evaluateSynonymWildCard(PKBClient* pkb, ClauseVariable left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsUsingSomeVar();

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
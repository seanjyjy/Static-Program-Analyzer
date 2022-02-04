#include "UsesSEvaluator.h"

Table* Evaluator::evaluate(string clause, PKB *pkb) {
    // Get first argument
    // Get second argument

    if (firstArgument.isInteger() && secondArgument.isIdentifier()) {
        return evaluateUsesIntegerIdentifier(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isInteger() && secondArgument.isSynonym()) {
        return evaluateUsesIntegerSynonym(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isInteger() && secondArgument.isWildCard()) {
        return evaluateUsesIntegerWildCard(pkb, firstArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isIdentifier()) {
        return evaluateUsesSynonymIdentifier(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isSynonym()) {
        return evaluateUsesSynonymSynonym(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isWildCard()) {
        return evaluateUsesSynonymWildCard(pkb, firstArgument);
    }

    return new FalseTable();
}

Table* UsesSEvaluator::evaluateUsesIntegerIdentifier(PKB* pkb, firstArgument, secondArgument) {
    bool isUsesS = pkb->isUsesS(firstArgument->getInteger(), secondArgument->getIdentifier());

    if (isUsesS) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* UsesSEvaluator::evaluateUsesIntegerSynonym(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfVariables = pkb->getUsesByStmt(firstArgument->getInteger());

    string column = secondArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesSEvaluator::evaluateUsesIntegerWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfVariables = pkb->getUsesByStmt(firstArgument->getInteger());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* UsesSEvaluator::evaluateUsesSynonymIdentifier(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfStatements =  pkb->getUsesSByVar(secondArgument->getIdentifier());

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* UsesSEvaluator::evaluateUsesSynonymSynonym(PKB* pkb, firstArgument, secondArgument) {
    vector<pair<string, string>> listOfStatementToVariable =  pkb->getAllUsesS();

    string firstColumn = firstArgument->getSynonym();
    string secondColumn = secondArgument->getSynonym();
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

Table* UsesSEvaluator::evaluateUsesSynonymWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsUsingSomeVar();

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
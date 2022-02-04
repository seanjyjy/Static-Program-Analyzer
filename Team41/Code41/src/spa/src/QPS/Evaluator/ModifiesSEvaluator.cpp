#include "ModifiesSEvaluator.h"

Table* ModifiesSEvaluator::evaluate(string clause, PKB *pkb) {
    // Get first argument
    // Get second argument

    if (firstArgument.isInteger() && secondArgument.isIdentifier()) {
        return evaluateModifiesIntegerIdentifier(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isInteger() && secondArgument.isSynonym()) {
        return evaluateModifiesIntegerSynonym(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isInteger() && secondArgument.isWildCard()) {
        return evaluateModifiesIntegerWildCard(pkb, firstArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isIdentifier()) {
        return evaluateModifiesSynonymIdentifier(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isSynonym()) {
        return evaluateModifiesSynonymSynonym(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isWildCard()) {
        return evaluateModifiesSynonymWildCard(pkb, firstArgument);
    }

    return new FalseTable();
}

Table* ModifiesSEvaluator::evaluateModifiesIntegerIdentifier(PKB* pkb, firstArgument, secondArgument) {
    bool isUsesS = pkb->isModifiesS(firstArgument->getInteger(), secondArgument->getIdentifier());

    if (isUsesS) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* ModifiesSEvaluator::evaluateModifiesIntegerSynonym(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfVariables = pkb->getModifiesByStmt(firstArgument->getInteger());

    string column = secondArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesSEvaluator::evaluateModifiesIntegerWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfVariables = pkb->getModifiesByStmt(firstArgument->getInteger());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* ModifiesSEvaluator::evaluateModifiesSynonymIdentifier(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfStatements =  pkb->getModifiesSByVar(secondArgument->getIdentifier());

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesSEvaluator::evaluateModifiesSynonymSynonym(PKB* pkb, firstArgument, secondArgument) {
    vector<pair<string, string>> listOfStatementToVariable =  pkb->getAllModifiesS();

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

Table* ModifiesSEvaluator::evaluateModifiesSynonymWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsModifyingSomeVar();

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
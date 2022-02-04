#include "ModfiesPEvaluator.h"

Table* ModifiesPEvaluator::evaluate(string clause, PKB* pkb) {
    // Get first argument
    // Get second argument

    if (firstArgument.isIdentifier() && secondArgument.isIdentifer()) {
        return evaluateUsesIdentifierIdentifier(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isIdentifier() && secondArgument.isSynonym()) {
        return evaluateUsesIdentifierSynonym(pkb, firstArgument, secondArgument);
    }

    if (firstArgument.isIdentifier() && secondArgument.isWildCard()) {
        return evaluateUsesIdentifierWildCard(pkb, firstArgument);
    }

    if (firstArgument.isSynonym() && secondArgument.isIdentifer()) {
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

Table* ModifiesPEvaluator::evaluateModifiesIdentifierIdentifier(PKB *pkb, firstArgument, secondArgument) {
    bool isModifiesP = pkb->isModifiesP(firstArgument->getIdentifier(), secondArgument->getIdentifier());

    if (isModifiesP) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* ModifiesPEvaluator::evaluateModifiesIdentifierSynonym(PKB *pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfVariables = pkb->getModifiesByProc(firstArgument->getIdentifier());

    string column = secondArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesPEvaluator::evaluateUsesIdentifierWildCard(PKB *pkb, firstArgument) {
    unordered_set<string> setOfVariables = pkb->getModifiesByProc(firstArgument->getIdentifier());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* ModifiesPEvaluator::evaluateUsesSynonymIdentifier(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfStatements = pkb->getModifiesPByVar(secondArgument->getIdentifier());

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesPEvaluator::evaluateUsesSynonymSynonym(PKB* pkb, firstArgument, secondArgument) {
    vector<pair<string, string>> listOfStatementToVariable = pkb->getAllModifiesP();

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

Table* ModifiesPEvaluator::evaluateUsesSynonymWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfStatements = pkb->getAllProcsModifyingSomeVar();

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
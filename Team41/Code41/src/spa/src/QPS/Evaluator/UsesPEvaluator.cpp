#include "UsesPEvaluator.h"

Table* Evaluator::evaluate(string clause, PKB* pkb) {
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

Table* UsesPEvaluator::evaluateUsesIdentifierIdentifier(PKB* pkb, firstArgument, secondArgument) {
    bool isUsesP = pkb->isUsesP(firstArgument->getIdentifier(), secondArgument->getIdentifier());

    if (isUsesP) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* UsesPEvaluator::evaluateUsesIdentifierSynonym(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(firstArgument->getIdentifier());

    string column = secondArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateUsesIdentifierWildCard(PKB* pkb, firstArgument) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(firstArgument->getIdentifier());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* UsesPEvaluator::evaluateUsesSynonymIdentifier(PKB* pkb, firstArgument, secondArgument) {
    unordered_set<string> setOfProcedures = pkb->getUsesPByVar(secondArgument->getIdentifier());

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateUsesSynonymSynonym(PKB* pkb, firstArgument, secondArgument) {
    vector<pair<string, string>> listOfProcedureToVariable =  pkb->getAllUsesP();

    string firstColumn = firstArgument->getSynonym();
    string secondColumn = secondArgument->getSynonym();
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

Table* UsesPEvaluator::evaluateUsesSynonymWildCard(PKB* pkb, firstArgument) {
    vector<pair<string, string>> listOfProcedureToVariable =  pkb->getAllUsesP();

    string column = firstArgument->getSynonym();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedureToVariable : listOfProcedureToVariable) {
        Row* row = new Row(column, procedureToVariable.first);
        result->addRow(row);
    }

    return result;
}
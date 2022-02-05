#include "ModifiesPEvaluator.h"

Table* ModifiesPEvaluator::evaluate(QueryClause clause, PKB* pkb) {
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

    return new FalseTable();
}

 Table* ModifiesPEvaluator::evaluateIdentifierIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    bool isModifiesP = pkb->isModifiesP(left.getLabel(), right.getLabel());

    if (isModifiesP) {
        return new TrueTable();
    }

    return new FalseTable();
}

 Table* ModifiesPEvaluator::evaluateIdentifierSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getModifiesByProc(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

 Table* ModifiesPEvaluator::evaluateIdentifierWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getModifiesByProc(left.getLabel());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

 Table* ModifiesPEvaluator::evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStatements = pkb->getModifiesPByVar(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesPEvaluator::evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStatementToVariable = pkb->getAllModifiesP();

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

Table* ModifiesPEvaluator::evaluateSynonymWildCard(PKB* pkb, ClauseVariable left) {
//    unordered_set<string> setOfStatements = pkb->getAllProcsModifyingSomeVar();

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

//    for (auto& statement : setOfStatements) {
//        Row* row = new Row(column, statement);
//        result->addRow(row);
//    }

    return result;
}
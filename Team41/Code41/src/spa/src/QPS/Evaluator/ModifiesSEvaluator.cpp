#include "ModifiesSEvaluator.h"

Table* ModifiesSEvaluator::evaluate(QueryClause clause, PKB *pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    auto lt = leftVariable.getType();
    auto rt = rightVariable.getType();
    printf("leftVariableType: %d\n", lt);
    printf("rightVariableType: %d\n", rt);

    printf("isLeftVariable synonym: %s\n", leftVariable.isSynonym() ? "YES" : "NO");
    printf("isrightVariable ident: %s\n", rightVariable.isIdentifier() ? "YES" : "NO");

    if (leftVariable.isInteger() && rightVariable.isIdentifier()) {
        return evaluateIntegerIdentifier(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isInteger() && rightVariable.isSynonym()) {
        return evaluateIntegerSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isInteger() && rightVariable.isWildCard()) {
        return evaluateIntegerWildCard(pkb, leftVariable);
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

Table* ModifiesSEvaluator::evaluateIntegerIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    bool isUsesS = pkb->isModifiesS(left.getLabel(), right.getLabel());

    if (isUsesS) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table* ModifiesSEvaluator::evaluateIntegerSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getModifiesByStmt(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesSEvaluator::evaluateIntegerWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getModifiesByStmt(left.getLabel());

    if (setOfVariables.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* ModifiesSEvaluator::evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStatements =  pkb->getModifiesSByVar(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}

Table* ModifiesSEvaluator::evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStatementToVariable =  pkb->getAllModifiesS();

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

Table* ModifiesSEvaluator::evaluateSynonymWildCard(PKB* pkb, ClauseVariable left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsModifyingSomeVar();

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
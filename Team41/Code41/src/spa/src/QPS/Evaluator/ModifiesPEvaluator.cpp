#include "ModifiesPEvaluator.h"

Table* ModifiesPEvaluator::evaluate(QueryClause clause, PKB* pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIdentifierIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(&leftVariable, &rightVariable)) {
        return evaluateIdentifierSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&leftVariable, &rightVariable)) {
        return evaluateIdentifierWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    // to be change to throw Semantic Error
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
    unordered_set<string> setOfStatements = pkb->getAllProcsModifyingSomeVar();

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        result->addRow(row);
    }

    return result;
}
#include "NextEvaluator.h"

Table *NextEvaluator::evaluate(QueryClause clause, PKBClient *pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&leftVariable, &rightVariable)) {
        return evaluateIntegerInteger(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&leftVariable, &rightVariable)) {
        return evaluateIntegerSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&leftVariable, &rightVariable)) {
        return evaluateIntegerWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&leftVariable, &rightVariable)) {
        return evaluateSynonymInteger(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&leftVariable, &rightVariable)) {
        return evaluateWildCardInteger(pkb, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&leftVariable, &rightVariable)) {
        return evaluateWildCardSynonym(pkb, rightVariable);
    }

    if (EvaluatorUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard(pkb);
    }

    throw SemanticException("Invalid query provided for Next");
}

Table *NextEvaluator::evaluateIntegerInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    bool isNext = pkb->isNext(left.getLabel(), right.getLabel());

    if (!isNext) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateIntegerSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    string nextStatement = pkb->getStatementAfter(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);
    Row* row = new Row(column, nextStatement);
    table->addRow(row);

    return table;
}

Table *NextEvaluator::evaluateIntegerWildCard(PKBClient *pkb, ClauseVariable left) {
    string nextStatement = pkb->getStatementAfter(left.getLabel());

    if (nextStatement.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateSynonymInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    string beforeStatement = pkb->getStatementBefore(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);
    Row* row = new Row(column, beforeStatement);
    table->addRow(row);

    return table;
}

Table *NextEvaluator::evaluateSynonymSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllNext();

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto& [beforeStatement, nextStatement] : listOfStmtToStmt) {
        Row* row = new Row();
        row->addEntry(firstColumn, beforeStatement);
        row->addEntry(secondColumn, nextStatement);
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateSynonymWildCard(PKBClient *pkb, ClauseVariable left) {
    vector<string> setOfStatements = pkb->getAllStmtsThatHaveNextStmt();

    if (setOfStatements.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateWildCardInteger(PKBClient *pkb, ClauseVariable right) {
    string beforeStatement = pkb->getStatementBefore(right.getLabel());

    if (beforeStatement.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateWildCardSynonym(PKBClient *pkb, ClauseVariable right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsThatIsNextOfSomeStmt();

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row();
        row->addEntry(column, statement);
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateWildCardWildCard(PKBClient *pkb) {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllNext();

    if (listOfStmtToStmt.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}
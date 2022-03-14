#include "NextTEvaluator.h"

Table *NextTEvaluator::evaluate(QueryClause clause, NextKBAdapter* nextKBAdapter) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&leftVariable, &rightVariable)) {
        return evaluateIntegerInteger(nextKBAdapter, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&leftVariable, &rightVariable)) {
        return evaluateIntegerSynonym(nextKBAdapter, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&leftVariable, &rightVariable)) {
        return evaluateIntegerWildCard(nextKBAdapter, leftVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&leftVariable, &rightVariable)) {
        return evaluateSynonymInteger(nextKBAdapter, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(nextKBAdapter, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(nextKBAdapter, leftVariable);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&leftVariable, &rightVariable)) {
        return evaluateWildCardInteger(nextKBAdapter, rightVariable);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&leftVariable, &rightVariable)) {
        return evaluateWildCardSynonym(nextKBAdapter, rightVariable);
    }

    if (EvaluatorUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard(nextKBAdapter);
    }

    throw SemanticException("Invalid query provided for NextT");
}

Table *NextTEvaluator::evaluateIntegerInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    if (!nextKBAdapter->isNextT(left.getLabel(), right.getLabel())) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextTEvaluator::evaluateIntegerSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& stmt : setOfStmts) {
        Row* row = new Row(column, stmt);
        table->addRow(row);
    }

    return table;
}

Table *NextTEvaluator::evaluateIntegerWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());

    if (setOfStmts.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextTEvaluator::evaluateSynonymInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& stmt : setOfStmts) {
        Row* row = new Row(column, stmt);
        table->addRow(row);
    }

    return table;
}

Table *NextTEvaluator::evaluateSynonymSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNextT();

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto&[firstStmt, secondStmt] : listOfStmtToStmt) {
        Row* row = new Row();
        row->addEntry(firstColumn, firstStmt);
        row->addEntry(secondColumn, secondStmt);
        table->addRow(row);
    }

    return table;
}

Table *NextTEvaluator::evaluateSynonymWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextTStmt();

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& stmt : setOfStmts) {
        Row* row = new Row(column, stmt);
        table->addRow(row);
    }

    return table;
}

Table *NextTEvaluator::evaluateWildCardInteger(NextKBAdapter* nextKBAdapter, ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());

    if (setOfStmts.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextTEvaluator::evaluateWildCardSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable right) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextTOfSomeStmt();

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& stmt: setOfStmts) {
        Row* row = new Row(column, stmt);
        table->addRow(row);
    }

    return table;
}

Table *NextTEvaluator::evaluateWildCardWildCard(NextKBAdapter* nextKBAdapter) {
    vector<pair<string, string>> listOfStmts = nextKBAdapter->getAllNextT();

    if (listOfStmts.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

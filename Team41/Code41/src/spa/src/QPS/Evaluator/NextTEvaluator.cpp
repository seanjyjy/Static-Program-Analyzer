#include "NextTEvaluator.h"

Table *NextTEvaluator::evaluate(QueryClause clause, NextKBAdapter* nextKBAdapter) {
    return nullptr;
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
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextTStmt();

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
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtThatIsNextTOfSomeStmt();

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

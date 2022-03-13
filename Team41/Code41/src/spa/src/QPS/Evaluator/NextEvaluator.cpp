#include "NextEvaluator.h"

Table *NextEvaluator::evaluate(QueryClause clause, NextKBAdapter* nextKBAdapter) {
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

    throw SemanticException("Invalid query provided for Next");
}

Table *NextEvaluator::evaluateIntegerInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    bool isNext = nextKBAdapter->isNext(left.getLabel(), right.getLabel());

    if (!isNext) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateIntegerSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    vector<CFGNode *> setOfChildren = nextKBAdapter->getNextNodes(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& child : setOfChildren) {
        Row* row = new Row(column, child->getStmtNum());
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateIntegerWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left) {
    vector<CFGNode *> setOfChildren = nextKBAdapter->getNextNodes(left.getLabel());

    if (setOfChildren.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateSynonymInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    vector<CFGNode *> setOfParent = nextKBAdapter->getPrevNodes(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& parent : setOfParent) {
        Row* row = new Row(column, parent->getStmtNum());
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateSynonymSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNext();

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

Table *NextEvaluator::evaluateSynonymWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextStmt();

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& stmt : setOfStmts) {
        Row* row = new Row(column, stmt);
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateWildCardInteger(NextKBAdapter* nextKBAdapter, ClauseVariable right) {
    vector<CFGNode *> setOfParent = nextKBAdapter->getPrevNodes(right.getLabel());

    if (setOfParent.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *NextEvaluator::evaluateWildCardSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable right) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextOfSomeStmt();

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStmts) {
        Row* row = new Row();
        row->addEntry(column, statement);
        table->addRow(row);
    }

    return table;
}

Table *NextEvaluator::evaluateWildCardWildCard(NextKBAdapter* nextKBAdapter) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNext();

    if (listOfStmtToStmt.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}
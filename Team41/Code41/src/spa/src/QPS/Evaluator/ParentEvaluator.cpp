#include "ParentEvaluator.h"

Table* ParentEvaluator::evaluate(QueryClause clause, PKB *pkb) {
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

    if (EvaluatorUtils::StmtUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard(pkb);
    }

    // to be change to throw Semantic Error
    return new FalseTable();
}

Table* ParentEvaluator::evaluateIntegerInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    bool isParent = pkb->isParent(left.getLabel(), right.getLabel());

    if (isParent) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* ParentEvaluator::evaluateIntegerSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());

    if (setOfChildren.empty()) {
        return new FalseTable();
    }

    string column = right.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& child : setOfChildren) {
        Row* row = new Row(column, child);
        table->addRow(row);
    }

    return table;
}

Table* ParentEvaluator::evaluateIntegerWildCard(PKB *pkb, ClauseVariable left) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());

    if (setOfChildren.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* ParentEvaluator::evaluateSynonymInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    string parent = pkb->getParentOf(right.getLabel());

    if (parent.empty()) {
        return new FalseTable();
    }

    string column = left.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    Row* row = new Row(column, parent);
    table->addRow(row);

    return table;
}

Table* ParentEvaluator::evaluateSynonymSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    // Gets list of parent-child pair where stmt1 is parent of stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();

    if (listOfStmtToStmt.empty()) {
        return new FalseTable();
    }

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto& stmtToStmt : listOfStmtToStmt) {
        Row* row = new Row();
        row->addEntry(firstColumn, stmtToStmt.first);
        row->addEntry(secondColumn, stmtToStmt.second);
        table->addRow(row);
    }

    return table;
}

Table* ParentEvaluator::evaluateSynonymWildCard(PKB *pkb, ClauseVariable left) {
    // get list of stmt parent of some stmt
    unordered_set<string> setOfParents = pkb->getAllStmtsParentOfSomeStmt();

    if (setOfParents.empty()) {
        return new FalseTable();
    }

    string column = left.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& parent : setOfParents) {
        Row* row = new Row(column, parent);
        table->addRow(row);
    }

    return table;
}

Table* ParentEvaluator::evaluateWildCardInteger(PKB *pkb, ClauseVariable right) {
    string parent = pkb->getParentOf(right.getLabel());

    if (parent.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* ParentEvaluator::evaluateWildCardSynonym(PKB *pkb, ClauseVariable right) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfParentedStmts = pkb->getAllStmtsChildOfBySomeStmt();

    if (setOfParentedStmts.empty()) {
        return new FalseTable();
    }

    string column = right.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& parentedStmt : setOfParentedStmts) {
        Row* row = new Row(column, parentedStmt);
        table->addRow(row);
    }

    return table;
}

Table* ParentEvaluator::evaluateWildCardWildCard(PKB *pkb) {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();

    if (listOfStmtToStmt.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

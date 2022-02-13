#include "ParentEvaluator.h"

Table* ParentEvaluator::evaluate(QueryClause clause, PKB *pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (leftVariable.isInteger() && rightVariable.isInteger()) {
        return evaluateIntegerInteger(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isInteger() && rightVariable.isSynonym()) {
        return evaluateIntegerSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isInteger() && rightVariable.isWildCard()) {
        return evaluateIntegerWildCard(pkb, leftVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isInteger()) {
        return evaluateSynonymInteger(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isSynonym()) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (leftVariable.isSynonym() && rightVariable.isWildCard()) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isInteger()) {
        return evaluateWildCardInteger(pkb, rightVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isSynonym()) {
        return evaluateWildCardSynonym(pkb, rightVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isWildCard()) {
        return evaluateWildCardWildCard(pkb);
    }

    return FalseTable::getTable();
}

Table* ParentEvaluator::evaluateIntegerInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    bool isParent = pkb->isParent(left.getLabel(), right.getLabel());

    if (isParent) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table* ParentEvaluator::evaluateIntegerSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());

    if (setOfChildren.empty()) {
        return FalseTable::getTable();
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
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* ParentEvaluator::evaluateSynonymInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    string parent = pkb->getParentOf(right.getLabel());

    if (parent.empty()) {
        return FalseTable::getTable();
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
        return FalseTable::getTable();
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
        return FalseTable::getTable();
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
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* ParentEvaluator::evaluateWildCardSynonym(PKB *pkb, ClauseVariable right) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfParentedStmts = pkb->getAllStmtsChildOfBySomeStmt();

    if (setOfParentedStmts.empty()) {
        return FalseTable::getTable();
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
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

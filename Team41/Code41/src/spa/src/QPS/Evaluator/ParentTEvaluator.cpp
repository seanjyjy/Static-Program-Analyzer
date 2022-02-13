#include "ParentTEvaluator.h"

Table* ParentTEvaluator::evaluate(QueryClause clause, PKB *pkb) {
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

    return FalseTable::getTable();
}

Table* ParentTEvaluator::evaluateIntegerInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    bool isParentT = pkb->isParentT(left.getLabel(), right.getLabel());

    if (isParentT) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table* ParentTEvaluator::evaluateIntegerSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfDescendants = pkb->getDescendantStmtsOf(left.getLabel());

    if (setOfDescendants.empty()) {
        return FalseTable::getTable();
    }

    string column = right.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& descendant : setOfDescendants) {
        Row* row = new Row(column, descendant);
        table->addRow(row);
    }

    return table;
}

Table* ParentTEvaluator::evaluateIntegerWildCard(PKB *pkb, ClauseVariable left) {
    unordered_set<string> setOfDescendants = pkb->getDescendantStmtsOf(left.getLabel());

    if (setOfDescendants.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* ParentTEvaluator::evaluateSynonymInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfAncestors = pkb->getAncestorStmtsOf(right.getLabel());

    if (setOfAncestors.empty()) {
        return FalseTable::getTable();
    }

    string column = left.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& ancestor : setOfAncestors) {
        Row* row = new Row(column, ancestor);
        table->addRow(row);
    }

    return table;
}

Table* ParentTEvaluator::evaluateSynonymSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    // Gets list of parent-child pair where stmt1 is ancestor of stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParentT();

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

Table* ParentTEvaluator::evaluateSynonymWildCard(PKB *pkb, ClauseVariable left) {
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

Table* ParentTEvaluator::evaluateWildCardInteger(PKB *pkb, ClauseVariable right) {
    unordered_set<string> setOfAncestors = pkb->getAncestorStmtsOf(right.getLabel());

    if (setOfAncestors.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* ParentTEvaluator::evaluateWildCardSynonym(PKB *pkb, ClauseVariable right) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfStmtsThatHasParent = pkb->getAllStmtsChildOfBySomeStmt();

    if (setOfStmtsThatHasParent.empty()) {
        return FalseTable::getTable();
    }

    string column = right.getLabel();
    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& stmtsThatHasParent : setOfStmtsThatHasParent) {
        Row* row = new Row(column, stmtsThatHasParent);
        table->addRow(row);
    }

    return table;
}

Table* ParentTEvaluator::evaluateWildCardWildCard(PKB *pkb) {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParentT();

    if (listOfStmtToStmt.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

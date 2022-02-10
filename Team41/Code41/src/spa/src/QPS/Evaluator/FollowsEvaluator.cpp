#include "FollowsEvaluator.h"

Table* FollowsEvaluator::evaluate(QueryClause clause, PKB *pkb) {
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

Table *FollowsEvaluator::evaluateIntegerInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    bool isFollows = pkb->isFollows(left.getLabel(), right.getLabel());

    if (isFollows) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table *FollowsEvaluator::evaluateIntegerSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    string follower = pkb->getStmtFollowing(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    if (follower.empty()) {
        return table;
    }

    Row* row = new Row(column, follower);
    table->addRow(row);

    return table;
}

Table *FollowsEvaluator::evaluateIntegerWildCard(PKB *pkb, ClauseVariable left) {
    string follower = pkb->getStmtFollowing(left.getLabel());

    if (follower.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table *FollowsEvaluator::evaluateSynonymInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    string followed = pkb->getStmtFollowedBy(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    if (followed.empty()) {
        return table;
    }

    Row* row = new Row(column, followed);
    table->addRow(row);

    return table;
}

Table *FollowsEvaluator::evaluateSynonymSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollows();

    string firstColumn = right.getLabel();
    string secondColumn = left.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto& stmtToStmt : listOfStmtToStmt) {
        Row* row = new Row();
        row->addEntry(firstColumn, stmtToStmt.first);
        row->addEntry(secondColumn, stmtToStmt.second);
        table->addRow(row);
    }

    return table;
}

Table *FollowsEvaluator::evaluateSynonymWildCard(PKB *pkb, ClauseVariable left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowedBySomeStmt();

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        table->addRow(row);
    }

    return table;
}

Table *FollowsEvaluator::evaluateWildCardInteger(PKB *pkb, ClauseVariable right) {
    string followedByCurrent = pkb->getStmtFollowedBy(right.getLabel());

    if (followedByCurrent.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table *FollowsEvaluator::evaluateWildCardSynonym(PKB *pkb, ClauseVariable right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowingSomeStmt();

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        table->addRow(row);
    }

    return table;
}

Table *FollowsEvaluator::evaluateWildCardWildCard(PKB *pkb) {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();

    if (listOfStmtStmt.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

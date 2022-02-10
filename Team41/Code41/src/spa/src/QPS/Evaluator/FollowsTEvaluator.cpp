#include "FollowsTEvaluator.h"

Table* FollowsTEvaluator::evaluate(QueryClause clause, PKB *pkb) {
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

Table* FollowsTEvaluator::evaluateIntegerInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    bool isFollows = pkb->isFollowsT(left.getLabel(), right.getLabel());

    if (isFollows) {
        return TrueTable::getTable();
    }

    return FalseTable::getTable();
}

Table* FollowsTEvaluator::evaluateIntegerSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> followers = pkb->getAllStmtsFollowingT(left.getLabel());

    if (followers.empty()) {
        return FalseTable::getTable();
    }

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& follower : followers) {
        Row* row = new Row(column, follower);
        table->addRow(row);
    }

    return table;
}

Table* FollowsTEvaluator::evaluateIntegerWildCard(PKB *pkb, ClauseVariable left) {
    unordered_set<string> followers = pkb->getAllStmtsFollowingT(left.getLabel());

    if (followers.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* FollowsTEvaluator::evaluateSynonymInteger(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> followedSet = pkb->getAllStmtsFollowedTBy(right.getLabel());

    if (followedSet.empty()) {
        return FalseTable::getTable();
    }

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& followed : followedSet) {
        Row* row = new Row(column, followed);
        table->addRow(row);

    }

    return table;
}

Table* FollowsTEvaluator::evaluateSynonymSynonym(PKB *pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollowsT();

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
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

Table* FollowsTEvaluator::evaluateSynonymWildCard(PKB *pkb, ClauseVariable left) {
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

Table* FollowsTEvaluator::evaluateWildCardInteger(PKB *pkb, ClauseVariable right) {
    unordered_set<string> followedByCurrent = pkb->getAllStmtsFollowedTBy(right.getLabel());

    if (followedByCurrent.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

Table* FollowsTEvaluator::evaluateWildCardSynonym(PKB *pkb, ClauseVariable right) {
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

Table* FollowsTEvaluator::evaluateWildCardWildCard(PKB *pkb) {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();

    if (listOfStmtStmt.empty()) {
        return FalseTable::getTable();
    }

    return TrueTable::getTable();
}

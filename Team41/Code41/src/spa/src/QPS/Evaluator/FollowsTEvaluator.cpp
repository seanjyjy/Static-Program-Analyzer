#include "FollowsTEvaluator.h"

Table* FollowsTEvaluator::evaluate(QueryClause clause, PKBClient *pkb) {
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

    throw SemanticException("Invalid query provided for Follows*");
}

Table* FollowsTEvaluator::evaluateIntegerInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    bool isFollows = pkb->isFollowsT(left.getLabel(), right.getLabel());

    if (isFollows) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* FollowsTEvaluator::evaluateIntegerSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> followers = pkb->getAllStmtsFollowingT(left.getLabel());

    if (followers.empty()) {
        return new FalseTable();
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

Table* FollowsTEvaluator::evaluateIntegerWildCard(PKBClient *pkb, ClauseVariable left) {
    unordered_set<string> followers = pkb->getAllStmtsFollowingT(left.getLabel());

    if (followers.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* FollowsTEvaluator::evaluateSynonymInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> followedSet = pkb->getAllStmtsFollowedTBy(right.getLabel());

    if (followedSet.empty()) {
        return new FalseTable();
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

Table* FollowsTEvaluator::evaluateSynonymSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollowsT();

    if (listOfStmtToStmt.empty()) {
        return new FalseTable();
    }

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

Table* FollowsTEvaluator::evaluateSynonymWildCard(PKBClient *pkb, ClauseVariable left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowedBySomeStmt();

    if (setOfStatements.empty()) {
        return new FalseTable();
    }

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        table->addRow(row);
    }

    return table;
}

Table* FollowsTEvaluator::evaluateWildCardInteger(PKBClient *pkb, ClauseVariable right) {
    unordered_set<string> followedByCurrent = pkb->getAllStmtsFollowedTBy(right.getLabel());

    if (followedByCurrent.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* FollowsTEvaluator::evaluateWildCardSynonym(PKBClient *pkb, ClauseVariable right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowingSomeStmt();

    if (setOfStatements.empty()) {
        return new FalseTable();
    }

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : setOfStatements) {
        Row* row = new Row(column, statement);
        table->addRow(row);
    }

    return table;
}

Table* FollowsTEvaluator::evaluateWildCardWildCard(PKBClient *pkb) {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollowsT();

    if (listOfStmtStmt.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

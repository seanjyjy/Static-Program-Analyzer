#include "FollowsEvaluator.h"

Table* FollowsEvaluator::evaluate(QueryClause clause, PKBClient *pkb) {
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

    throw SemanticException("Invalid query provided for Follows");
}

Table *FollowsEvaluator::evaluateIntegerInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    bool isFollows = pkb->isFollows(left.getLabel(), right.getLabel());

    if (isFollows) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table *FollowsEvaluator::evaluateIntegerSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    string follower = pkb->getStmtFollowing(left.getLabel());

    if (follower.empty()) {
        return new FalseTable();
    }

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);
    Row* row = new Row(column, follower);
    table->addRow(row);

    return table;
}

Table *FollowsEvaluator::evaluateIntegerWildCard(PKBClient *pkb, ClauseVariable left) {
    string follower = pkb->getStmtFollowing(left.getLabel());

    if (follower.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *FollowsEvaluator::evaluateSynonymInteger(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    string followed = pkb->getStmtFollowedBy(right.getLabel());

    if (followed.empty()) {
        return new FalseTable();
    }

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);
    Row* row = new Row(column, followed);
    table->addRow(row);

    return table;
}

Table *FollowsEvaluator::evaluateSynonymSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollows();

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

Table *FollowsEvaluator::evaluateSynonymWildCard(PKBClient *pkb, ClauseVariable left) {
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

Table *FollowsEvaluator::evaluateWildCardInteger(PKBClient *pkb, ClauseVariable right) {
    string followedByCurrent = pkb->getStmtFollowedBy(right.getLabel());

    if (followedByCurrent.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *FollowsEvaluator::evaluateWildCardSynonym(PKBClient *pkb, ClauseVariable right) {
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

Table *FollowsEvaluator::evaluateWildCardWildCard(PKBClient *pkb) {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();

    if (listOfStmtStmt.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

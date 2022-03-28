#include "FollowsEvaluator.h"

FollowsEvaluator::FollowsEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *FollowsEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isFollows = pkb->isFollows(left.getLabel(), right.getLabel());
    return buildBooleanTable(isFollows);
}

Table *FollowsEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right) {
    string follower = pkb->getStmtDirectlyAfter(left.getLabel());
    return buildSingleSynonymTable(follower, right);
}

Table *FollowsEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> beforeStmts = pkb->getAllStmtsBeforeSomeStmt();
    return buildBooleanTable(beforeStmts.find(left.getLabel()) != beforeStmts.end());
}

Table *FollowsEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right) {
    string followed = pkb->getStmtDirectlyBefore(right.getLabel());
    return buildSingleSynonymTable(followed, left);
}

Table *FollowsEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollows();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *FollowsEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsBeforeSomeStmt();
    return buildSingleSynonymTable(setOfStatements, left);
}

Table *FollowsEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    unordered_set<string> afterStmts = pkb->getAllStmtsAfterSomeStmt();
    return buildBooleanTable(afterStmts.find(right.getLabel()) != afterStmts.end());
}

Table *FollowsEvaluator::evaluateWildCardSynonym(ClauseVariable &right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsAfterSomeStmt();
    return buildSingleSynonymTable(setOfStatements, right);
}

Table *FollowsEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();
    return buildBooleanTable(!listOfStmtStmt.empty());
}
#include "FollowsEvaluator.h"

FollowsEvaluator::FollowsEvaluator(PKBClient *pkb): StmtStmtEvaluator(pkb) {}

Table *FollowsEvaluator::evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) {
    bool isFollows = pkb->isFollows(left.getLabel(), right.getLabel());
    return buildBooleanTable(isFollows);
}

Table *FollowsEvaluator::evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) {
    string follower = pkb->getStmtFollowing(left.getLabel());
    return buildSingleSynonymTable(follower, right);
}

Table *FollowsEvaluator::evaluateIntegerWildCard(ClauseVariable left) {
    string follower = pkb->getStmtFollowing(left.getLabel());
    return buildBooleanTable(!follower.empty());
}

Table *FollowsEvaluator::evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) {
    string followed = pkb->getStmtFollowedBy(right.getLabel());
    return buildSingleSynonymTable(followed, left);
}

Table *FollowsEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollows();
    // have to flip
    return buildSynonymSynonymTable(listOfStmtToStmt, right, left);
}

Table *FollowsEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowedBySomeStmt();
    return buildSingleSynonymTable(setOfStatements, left);
}

Table *FollowsEvaluator::evaluateWildCardInteger(ClauseVariable right) {
    string followedByCurrent = pkb->getStmtFollowedBy(right.getLabel());
    return buildBooleanTable(!followedByCurrent.empty());
}

Table *FollowsEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsFollowingSomeStmt();
    return buildSingleSynonymTable(setOfStatements, right);
}

Table *FollowsEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();
    return buildBooleanTable(!listOfStmtStmt.empty());
}
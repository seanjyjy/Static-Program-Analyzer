#include "FollowsTEvaluator.h"

FollowsTEvaluator::FollowsTEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *FollowsTEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isFollows = pkb->isFollowsT(left.getLabel(), right.getLabel());
    return buildBooleanTable(isFollows);
}

Table *FollowsTEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> followers = pkb->getAllStmtsAfter(left.getLabel());
    return buildSingleSynonymTable(followers, right);
}

Table *FollowsTEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> followers = pkb->getAllStmtsAfter(left.getLabel());
    return buildBooleanTable(!followers.empty());
}

Table *FollowsTEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> followedSet = pkb->getAllStmtsBefore(right.getLabel());
    return buildSingleSynonymTable(followedSet, left);
}

Table *FollowsTEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollowsT();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *FollowsTEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsBeforeSomeStmt();
    return buildSingleSynonymTable(setOfStatements, left);
}

Table *FollowsTEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    unordered_set<string> followedByCurrent = pkb->getAllStmtsBefore(right.getLabel());
    return buildBooleanTable(!followedByCurrent.empty());
}

Table *FollowsTEvaluator::evaluateWildCardSynonym(ClauseVariable &right) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsAfterSomeStmt();
    return buildSingleSynonymTable(setOfStatements, right);
}

Table *FollowsTEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollowsT();
    return buildBooleanTable(!listOfStmtStmt.empty());
}

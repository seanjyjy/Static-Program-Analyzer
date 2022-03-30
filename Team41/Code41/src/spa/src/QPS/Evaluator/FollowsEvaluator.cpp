#include "FollowsEvaluator.h"

FollowsEvaluator::FollowsEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *FollowsEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isFollows = pkb->isFollows(left.getLabel(), right.getLabel());
    return buildBooleanTable(isFollows);
}

Table *FollowsEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    string follower = pkb->getStmtDirectlyAfter(left.getLabel());
    return buildSingleSynonymTable(follower, right, canSimplify);
}

Table *FollowsEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> beforeStmts = pkb->getAllStmtsBeforeSomeStmt();
    return buildBooleanTable(beforeStmts.find(left.getLabel()) != beforeStmts.end());
}

Table *FollowsEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    string followed = pkb->getStmtDirectlyBefore(right.getLabel());
    return buildSingleSynonymTable(followed, left, canSimplify);
}

Table *FollowsEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    // returns stmt1 follows stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllFollows();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right, canSimplify);
}

Table *FollowsEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsBeforeSomeStmt();
    return buildSingleSynonymTable(setOfStatements, left, canSimplify);
}

Table *FollowsEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    unordered_set<string> afterStmts = pkb->getAllStmtsAfterSomeStmt();
    return buildBooleanTable(afterStmts.find(right.getLabel()) != afterStmts.end());
}

Table *FollowsEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    unordered_set<string> setOfStatements = pkb->getAllStmtsAfterSomeStmt();
    return buildSingleSynonymTable(setOfStatements, right, canSimplify);
}

Table *FollowsEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtStmt = pkb->getAllFollows();
    return buildBooleanTable(!listOfStmtStmt.empty());
}
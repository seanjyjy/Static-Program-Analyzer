#include "ParentEvaluator.h"

ParentEvaluator::ParentEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *ParentEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isParent = pkb->isParent(left.getLabel(), right.getLabel());
    return buildBooleanTable(isParent);
}

Table *ParentEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());
    return buildSingleSynonymTable(setOfChildren, right);
}

Table *ParentEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());
    return buildBooleanTable(!setOfChildren.empty());
}

Table *ParentEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right) {
    string parent = pkb->getParentOf(right.getLabel());
    return buildSingleSynonymTable(parent, left);
}

Table *ParentEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    // Gets list of parent-child pair where stmt1 is parent of stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *ParentEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    // get list of stmt parent of some stmt
    unordered_set<string> setOfParents = pkb->getAllStmtsParentOfSomeStmt();
    return buildSingleSynonymTable(setOfParents, left);
}

Table *ParentEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    string parent = pkb->getParentOf(right.getLabel());
    return buildBooleanTable(!parent.empty());
}

Table *ParentEvaluator::evaluateWildCardSynonym(ClauseVariable &right) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfParentedStmts = pkb->getAllStmtsChildOfSomeStmt();
    return buildSingleSynonymTable(setOfParentedStmts, right);
}

Table *ParentEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();
    return buildBooleanTable(!listOfStmtToStmt.empty());
}


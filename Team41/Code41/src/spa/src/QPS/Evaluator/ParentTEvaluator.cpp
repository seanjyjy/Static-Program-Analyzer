#include "ParentTEvaluator.h"

ParentTEvaluator::ParentTEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *ParentTEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isParentT = pkb->isParentT(left.getLabel(), right.getLabel());
    return buildBooleanTable(isParentT);
}

Table *ParentTEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> setOfDescendants = pkb->getDescendantStmtsOf(left.getLabel());
    return buildSingleSynonymTable(setOfDescendants, right);
}

Table *ParentTEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> parents = pkb->getAllStmtsParentOfSomeStmt();
    return buildBooleanTable(parents.find(left.getLabel()) != parents.end());
}

Table *ParentTEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> setOfAncestors = pkb->getAncestorStmtsOf(right.getLabel());
    return buildSingleSynonymTable(setOfAncestors, left);
}

Table *ParentTEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    // Gets list of parent-child pair where stmt1 is ancestor of stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParentT();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *ParentTEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    // get list of stmt parent of some stmt
    unordered_set<string> setOfParents = pkb->getAllStmtsParentOfSomeStmt();
    return buildSingleSynonymTable(setOfParents, left);
}

Table *ParentTEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    unordered_set<string> children = pkb->getAllStmtsChildOfSomeStmt();
    return buildBooleanTable(children.find(right.getLabel()) != children.end());
}

Table *ParentTEvaluator::evaluateWildCardSynonym(ClauseVariable &right) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfStmtsThatHasParent = pkb->getAllStmtsChildOfSomeStmt();
    return buildSingleSynonymTable(setOfStmtsThatHasParent, right);
}

Table *ParentTEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParentT();
    return buildBooleanTable(!listOfStmtToStmt.empty());
}
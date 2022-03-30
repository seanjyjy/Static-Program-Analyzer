#include "ParentEvaluator.h"

ParentEvaluator::ParentEvaluator(PKBClient *pkb) : StmtStmtEvaluator(pkb) {}

Table *ParentEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isParent = pkb->isParent(left.getLabel(), right.getLabel());
    return buildBooleanTable(isParent);
}

Table *ParentEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    unordered_set<string> setOfChildren = pkb->getChildStmtsOf(left.getLabel());
    return buildSingleSynonymTable(setOfChildren, right, canSimplify);
}

Table *ParentEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    unordered_set<string> parents = pkb->getAllStmtsParentOfSomeStmt();
    return buildBooleanTable(parents.find(left.getLabel()) != parents.end());
}

Table *ParentEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    string parent = pkb->getParentOf(right.getLabel());
    return buildSingleSynonymTable(parent, left, canSimplify);
}

Table *ParentEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    // Gets list of parent-child pair where stmt1 is parent of stmt2
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right, canSimplify);
}

Table *ParentEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    // get list of stmt parent of some stmt
    unordered_set<string> setOfParents = pkb->getAllStmtsParentOfSomeStmt();
    return buildSingleSynonymTable(setOfParents, left, canSimplify);
}

Table *ParentEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    unordered_set<string> children = pkb->getAllStmtsChildOfSomeStmt();
    return buildBooleanTable(children.find(right.getLabel()) != children.end());
}

Table *ParentEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    // get list of stmt parented by some stmt
    unordered_set<string> setOfParentedStmts = pkb->getAllStmtsChildOfSomeStmt();
    return buildSingleSynonymTable(setOfParentedStmts, right, canSimplify);
}

Table *ParentEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtToStmt = pkb->getAllParent();
    return buildBooleanTable(!listOfStmtToStmt.empty());
}


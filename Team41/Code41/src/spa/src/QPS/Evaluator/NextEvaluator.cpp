#include "NextEvaluator.h"

NextEvaluator::NextEvaluator(PKBClient* pkb, NextKBAdapter* nextKBAdapter): StmtStmtEvaluator(pkb, nextKBAdapter) {}

Table *NextEvaluator::evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) {
    bool isNext = nextKBAdapter->isNext(left.getLabel(), right.getLabel());
    return buildBooleanTable(isNext);
}

Table *NextEvaluator::evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) {
    vector<CFGNode *> setOfChildren = nextKBAdapter->getNextNodes(left.getLabel());
    return buildSingleSynonymTable(setOfChildren, right);
}

Table *NextEvaluator::evaluateIntegerWildCard(ClauseVariable left) {
    vector<CFGNode *> setOfChildren = nextKBAdapter->getNextNodes(left.getLabel());
    return buildBooleanTable(setOfChildren);
}

Table *NextEvaluator::evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) {
    vector<CFGNode *> setOfParent = nextKBAdapter->getPrevNodes(right.getLabel());
    return buildSingleSynonymTable(setOfParent, left);
}

Table *NextEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNext();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *NextEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextStmt();
    return buildSingleSynonymTable(setOfStmts, left);
}

Table *NextEvaluator::evaluateWildCardInteger(ClauseVariable right) {
    vector<CFGNode *> setOfParent = nextKBAdapter->getPrevNodes(right.getLabel());
    return buildBooleanTable(setOfParent);
}

Table *NextEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextOfSomeStmt();
    return buildSingleSynonymTable(setOfStmts, right);
}

Table *NextEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNext();
    return buildBooleanTable(listOfStmtToStmt);
}

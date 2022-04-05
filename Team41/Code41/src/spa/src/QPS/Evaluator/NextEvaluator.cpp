#include "NextEvaluator.h"

NextEvaluator::NextEvaluator(PKBClient *pkb, NextKBProxy *nextKBProxy) : StmtStmtEvaluator(pkb) {
    this->nextKBProxy = nextKBProxy;
}

Table *NextEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    bool isNext = nextKBProxy->isNext(left.getLabel(), right.getLabel());
    return buildBooleanTable(isNext);
}

Table *NextEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<CFGNode *> setOfChildren = nextKBProxy->getNextNodes(left.getLabel());
    return buildSingleSynonymTable(setOfChildren, right, canSimplify);
}

Table *NextEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    vector<CFGNode *> setOfChildren = nextKBProxy->getNextNodes(left.getLabel());
    return buildBooleanTable(!setOfChildren.empty());
}

Table *NextEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<CFGNode *> setOfParent = nextKBProxy->getPrevNodes(right.getLabel());
    return buildSingleSynonymTable(setOfParent, left, canSimplify);
}

Table *NextEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBProxy->getAllNext();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right, canSimplify);
}

Table *NextEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    vector<string> setOfStmts = nextKBProxy->getAllStmtsThatHaveNextStmt();
    return buildSingleSynonymTable(setOfStmts, left, canSimplify);
}

Table *NextEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    vector<CFGNode *> setOfParent = nextKBProxy->getPrevNodes(right.getLabel());
    return buildBooleanTable(!setOfParent.empty());
}

Table *NextEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    vector<string> setOfStmts = nextKBProxy->getAllStmtsThatIsNextOfSomeStmt();
    return buildSingleSynonymTable(setOfStmts, right, canSimplify);
}

Table *NextEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmtToStmt = nextKBProxy->getAllNext();
    return buildBooleanTable(!listOfStmtToStmt.empty());
}

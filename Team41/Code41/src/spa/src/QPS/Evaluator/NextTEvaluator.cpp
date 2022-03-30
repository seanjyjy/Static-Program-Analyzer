#include "NextTEvaluator.h"

NextTEvaluator::NextTEvaluator(PKBClient *pkb, NextKBAdapter *nextKBAdapter) : StmtStmtEvaluator(pkb) {
    this->nextKBAdapter = nextKBAdapter;
}

Table *NextTEvaluator::evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) {
    if (!EvaluatorUtils::isWithinLimit(left, right, pkb))
        return new FalseTable();

    bool isNextT = nextKBAdapter->isNextT(left.getLabel(), right.getLabel());
    return buildBooleanTable(isNextT);
}

Table *NextTEvaluator::evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (!EvaluatorUtils::isWithinLimit(left, pkb))
        return new FalseTable();

    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());
    return buildSingleSynonymTable(setOfStmts, right, canSimplify);
}

Table *NextTEvaluator::evaluateIntegerWildCard(ClauseVariable &left) {
    vector<CFGNode *> children = nextKBAdapter->getNextNodes(left.getLabel());
    return buildBooleanTable(!children.empty());
}

Table *NextTEvaluator::evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (!EvaluatorUtils::isWithinLimit(right, pkb))
        return new FalseTable();

    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());
    return buildSingleSynonymTable(setOfStmts, left, canSimplify);
}

Table *NextTEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNextT();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right, canSimplify);
}

Table *NextTEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextTStmt();
    return buildSingleSynonymTable(setOfStmts, left, canSimplify);
}

Table *NextTEvaluator::evaluateWildCardInteger(ClauseVariable &right) {
    vector<CFGNode *> parent = nextKBAdapter->getPrevNodes(right.getLabel());
    return buildBooleanTable(!parent.empty());
}

Table *NextTEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextTOfSomeStmt();
    return buildSingleSynonymTable(setOfStmts, right, canSimplify);
}

Table *NextTEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmts = nextKBAdapter->getAllNext();
    return buildBooleanTable(!listOfStmts.empty());
}
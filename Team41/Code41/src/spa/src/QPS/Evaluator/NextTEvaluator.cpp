#include "NextTEvaluator.h"

NextTEvaluator::NextTEvaluator(NextKBAdapter *nextKBAdapter) {
    this->nextKBAdapter = nextKBAdapter;
}

Table *NextTEvaluator::evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) {
    bool isNextT = nextKBAdapter->isNext(left.getLabel(), right.getLabel());
    return buildBooleanTable(isNextT);
}

Table *NextTEvaluator::evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());
    return buildSingleSynonymTable(setOfStmts, right);
}

Table *NextTEvaluator::evaluateIntegerWildCard(ClauseVariable left) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());
    return buildBooleanTable(setOfStmts);
}

Table *NextTEvaluator::evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());
    return buildSingleSynonymTable(setOfStmts, left);
}

Table *NextTEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfStmtToStmt = nextKBAdapter->getAllNextT();
    return buildSynonymSynonymTable(listOfStmtToStmt, left, right);
}

Table *NextTEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatHaveNextTStmt();
    return buildSingleSynonymTable(setOfStmts, left);
}

Table *NextTEvaluator::evaluateWildCardInteger(ClauseVariable right) {
    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());
    return buildBooleanTable(setOfStmts);
}

Table *NextTEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextTOfSomeStmt();
    return buildSingleSynonymTable(setOfStmts, right);
}

Table *NextTEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmts = nextKBAdapter->getAllNextT();
    return buildBooleanTable(listOfStmts);
}
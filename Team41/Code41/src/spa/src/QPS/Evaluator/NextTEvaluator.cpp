#include "NextTEvaluator.h"

NextTEvaluator::NextTEvaluator(PKBClient *pkb, NextKBAdapter *nextKBAdapter) : StmtStmtEvaluator(pkb) {
    this->nextKBAdapter = nextKBAdapter;
}

Table *NextTEvaluator::evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) {
    if (!EvaluatorUtils::isWithinLimit(left, right, pkb))
        return new FalseTable();

    bool isNextT = nextKBAdapter->isNextT(left.getLabel(), right.getLabel());
    return buildBooleanTable(isNextT);
}

Table *NextTEvaluator::evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) {
    if (!EvaluatorUtils::isWithinLimit(left, pkb))
        return new FalseTable();

    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());
    return buildSingleSynonymTable(setOfStmts, right);
}

Table *NextTEvaluator::evaluateIntegerWildCard(ClauseVariable left) {
    if (!EvaluatorUtils::isWithinLimit(left, pkb))
        return new FalseTable();

    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsNextT(left.getLabel());
    return buildBooleanTable(!setOfStmts.empty());
}

Table *NextTEvaluator::evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) {
    if (!EvaluatorUtils::isWithinLimit(right, pkb))
        return new FalseTable();

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
    if (!EvaluatorUtils::isWithinLimit(right, pkb))
        return new FalseTable();

    unordered_set<string> setOfStmts = nextKBAdapter->getAllStmtsTBefore(right.getLabel());
    return buildBooleanTable(!setOfStmts.empty());
}

Table *NextTEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    vector<string> setOfStmts = nextKBAdapter->getAllStmtsThatIsNextTOfSomeStmt();
    return buildSingleSynonymTable(setOfStmts, right);
}

Table *NextTEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfStmts = nextKBAdapter->getAllNext();
    return buildBooleanTable(!listOfStmts.empty());
}
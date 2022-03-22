#include "AffectsEvaluator.h"

/*
Affects(a1, a2)

 assignment statement vs assignment statement only

 (must check if both are assignment statement)

 integer vs integer -> BOOLEAN TABLE
 integer vs synonym -> SINGLE SYNONYM TABLE
 integer vs wildcard -> BOOLEAN TABLE

 synonym vs integer -> SINGLE SYNONYM TABLE
 synonym vs synonym -> DOUBLE SYNONYM TABLE ***
 synonym vs wildcard -> BOOLEAN TABLE (find any assignment statements that affects someone)

 wildcard vs integer -> BOOLEAN TABLE
 wildcard vs synonym -> SINGLE SYNONYM TABLE (find any assignment statement that is affected by someone)
 wildcard vs wildcard -> BOOLEAN TABLE
 */
AffectsEvaluator::AffectsEvaluator(PKBClient *pkb, AffectsKBAdapter *affectsKBAdapter): StmtStmtEvaluator(pkb) {
    this->affectsKBAdapter = affectsKBAdapter;
}

Table *AffectsEvaluator::evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) {
    bool isAffects = affectsKBAdapter->isAffects(left.getLabel(), right.getLabel());
    return buildBooleanTable(isAffects);
}

Table *AffectsEvaluator::evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> results = affectsKBAdapter->getDirectAffectsBy(left.getLabel());
    return buildSingleSynonymTable(results, right);
}

Table *AffectsEvaluator::evaluateIntegerWildCard(ClauseVariable left) {
    unordered_set<string> results = affectsKBAdapter->getDirectAffectsBy(left.getLabel());
    return buildBooleanTable(!results.empty());
}

Table *AffectsEvaluator::evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> results = affectsKBAdapter->getDirectAffecting(right.getLabel());
    return buildSingleSynonymTable(results, left);
}

Table *AffectsEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> results = affectsKBAdapter->getDirectAffectsAll();
    return buildSynonymSynonymTable(results, left, right);
}

Table *AffectsEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    unordered_set<string> results = affectsKBAdapter->getAllStmtAffectingOther();
    return buildSingleSynonymTable(results, left);
}

Table *AffectsEvaluator::evaluateWildCardInteger(ClauseVariable right) {
    unordered_set<string> results = affectsKBAdapter->getDirectAffecting(right.getLabel());
    return buildBooleanTable(!results.empty());
}

Table *AffectsEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    unordered_set<string> results = affectsKBAdapter->getAllStmtAffectedByOther();
    return buildSingleSynonymTable(results, right);
}

Table *AffectsEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> results = affectsKBAdapter->getDirectAffectsAll();
    return buildBooleanTable(!results.empty());
}

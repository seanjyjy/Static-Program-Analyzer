#include "AffectsEvaluator.h"

AffectsEvaluator::AffectsEvaluator(PKBClient *pkb, AffectsKBAdapter *affectsKBAdapter)
        : AffectEvaluator(pkb, affectsKBAdapter) {}

bool AffectsEvaluator::getIntegerIntegerRelation(const string &left, const string &right) {
    return affectsKBAdapter->isAffects(left, right);
}

unordered_set<string> AffectsEvaluator::getIntegerSynonymRelation(const string &label) {
    return affectsKBAdapter->getDirectAffectsBy(label);
}

unordered_set<string> AffectsEvaluator::getSynonymIntegerRelation(const string &label) {
    return affectsKBAdapter->getDirectAffecting(label);
}

vector<pair<string, string>> AffectsEvaluator::getSynonymSynonymRelation() {
    return affectsKBAdapter->getDirectAffectsAll();
}

unordered_set<string> AffectsEvaluator::getSynonymWildCardRelation() {
    return affectsKBAdapter->getAllStmtAffectingOther();
}

unordered_set<string> AffectsEvaluator::getWildCardSynonymRelation() {
    return affectsKBAdapter->getAllStmtAffectedByOther();
}
#include "AffectsEvaluator.h"

AffectsEvaluator::AffectsEvaluator(PKBClient *pkb, AffectsKBProxy *affectsKBProxy)
        : AffectEvaluator(pkb, affectsKBProxy) {}

bool AffectsEvaluator::getIntegerIntegerRelation(const string &left, const string &right) {
    return affectsKBProxy->isAffects(left, right);
}

unordered_set<string> AffectsEvaluator::getIntegerSynonymRelation(const string &label) {
    return affectsKBProxy->getDirectAffectsBy(label);
}

unordered_set<string> AffectsEvaluator::getSynonymIntegerRelation(const string &label) {
    return affectsKBProxy->getDirectAffecting(label);
}

vector<pair<string, string>> AffectsEvaluator::getSynonymSynonymRelation() {
    return affectsKBProxy->getDirectAffectsAll();
}

unordered_set<string> AffectsEvaluator::getSynonymWildCardRelation() {
    return affectsKBProxy->getAllStmtAffectingOther();
}

unordered_set<string> AffectsEvaluator::getWildCardSynonymRelation() {
    return affectsKBProxy->getAllStmtAffectedByOther();
}
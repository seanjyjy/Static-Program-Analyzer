#include "AffectsTEvaluator.h"

AffectsTEvaluator::AffectsTEvaluator(PKBClient *pkb, AffectsKBAdapter *affectsKBAdapter): AffectEvaluator(pkb) {
    this->affectsKBAdapter = affectsKBAdapter;
}

bool AffectsTEvaluator::getIntegerIntegerRelation(const string &left, const string &right) {
    return affectsKBAdapter->isAffectsT(left, right);
}

unordered_set<string> AffectsTEvaluator::getIntegerSynonymRelation(const string &label) {
    return affectsKBAdapter->getAffectsTBy(label);
}

unordered_set<string> AffectsTEvaluator::getIntegerWildCardRelation(const string &label) {
    return affectsKBAdapter->getAffectsTBy(label);
}

unordered_set<string> AffectsTEvaluator::getSynonymIntegerRelation(const string &label) {
    return affectsKBAdapter->getAffectingT(label);
}

vector<pair<string, string>> AffectsTEvaluator::getSynonymSynonymRelation() {
    return affectsKBAdapter->getAffectsTAll();
}

unordered_set<string> AffectsTEvaluator::getSynonymWildCardRelation() {
    return affectsKBAdapter->getAllStmtAffectingTOther();
}

unordered_set<string> AffectsTEvaluator::getWildCardIntegerRelation(const string &label) {
    return affectsKBAdapter->getAffectingT(label);
}

unordered_set<string> AffectsTEvaluator::getWildCardSynonymRelation() {
    return affectsKBAdapter->getAllStmtAffectedTByOther();
}

vector<pair<string, string>> AffectsTEvaluator::getWildCardWildCardRelation() {
    return affectsKBAdapter->getAffectsTAll();
}



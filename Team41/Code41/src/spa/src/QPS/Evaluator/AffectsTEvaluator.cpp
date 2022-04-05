#include "AffectsTEvaluator.h"

AffectsTEvaluator::AffectsTEvaluator(PKBClient *pkb, AffectsKBProxy *affectsKBProxy)
        : AffectEvaluator(pkb, affectsKBProxy) {}

bool AffectsTEvaluator::getIntegerIntegerRelation(const string &left, const string &right) {
    return affectsKBProxy->isAffectsT(left, right);
}

unordered_set<string> AffectsTEvaluator::getIntegerSynonymRelation(const string &label) {
    return affectsKBProxy->getAffectsTBy(label);
}

unordered_set<string> AffectsTEvaluator::getSynonymIntegerRelation(const string &label) {
    return affectsKBProxy->getAffectingT(label);
}

vector<pair<string, string>> AffectsTEvaluator::getSynonymSynonymRelation() {
    return affectsKBProxy->getAffectsTAll();
}

unordered_set<string> AffectsTEvaluator::getSynonymWildCardRelation() {
    return affectsKBProxy->getAllStmtAffectingTOther();
}

unordered_set<string> AffectsTEvaluator::getWildCardSynonymRelation() {
    return affectsKBProxy->getAllStmtAffectedTByOther();
}

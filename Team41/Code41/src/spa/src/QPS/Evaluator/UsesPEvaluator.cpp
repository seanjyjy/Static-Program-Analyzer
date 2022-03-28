#include "UsesPEvaluator.h"

UsesPEvaluator::UsesPEvaluator(PKBClient *pkb) : ModifiesUsesPEvaluator(pkb) {}

bool UsesPEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isUsesP(leftLabel, rightLabel);
}

unordered_set<string> UsesPEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getUsesByProc(label);
}

bool UsesPEvaluator::getIdentifierWildCardRelation(const string &label) {
    unordered_set<string> allProcs = pkb->getAllProcsUsingSomeVar();
    return allProcs.find(label) != allProcs.end();
}

unordered_set<string> UsesPEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getUsesPByVar(label);
}

vector<pair<string, string>> UsesPEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllUsesP();
}

unordered_set<string> UsesPEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllProcsUsingSomeVar();
}

bool UsesPEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allVar = pkb->getAllVarUsedBySomeProc();
    return allVar.find(label) != allVar.end();
}

bool UsesPEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllUsesP().empty();
}

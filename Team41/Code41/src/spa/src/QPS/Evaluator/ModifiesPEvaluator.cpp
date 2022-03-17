#include "ModifiesPEvaluator.h"

ModifiesPEvaluator::ModifiesPEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
}

bool ModifiesPEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isModifiesP(leftLabel, rightLabel);
}

unordered_set<string> ModifiesPEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getModifiesByProc(label);
}

unordered_set<string> ModifiesPEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getModifiesByProc(label);
}

unordered_set<string> ModifiesPEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getModifiesPByVar(label);
}

vector<pair<string, string>> ModifiesPEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllModifiesP();
}

unordered_set<string> ModifiesPEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllProcsModifyingSomeVar();
}
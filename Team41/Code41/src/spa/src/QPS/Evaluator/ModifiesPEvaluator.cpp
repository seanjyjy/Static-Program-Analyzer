#include "ModifiesPEvaluator.h"

ModifiesPEvaluator::ModifiesPEvaluator(PKBClient *pkb) : ModifiesUsesPEvaluator(pkb) {}

bool ModifiesPEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isModifiesP(leftLabel, rightLabel);
}

unordered_set<string> ModifiesPEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getModifiesByProc(label);
}

bool ModifiesPEvaluator::getIdentifierWildCardRelation(const string &label) {
    unordered_set<string> allProc = pkb->getAllProcsModifyingSomeVar();
    return allProc.find(label) != allProc.end();
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

bool ModifiesPEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allVar = pkb->getAllVarModifiedBySomeProc();
    return allVar.find(label) != allVar.end();
}

bool ModifiesPEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllModifiesP().empty();
}

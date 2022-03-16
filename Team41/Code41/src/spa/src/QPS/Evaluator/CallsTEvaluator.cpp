#include "CallsTEvaluator.h"

CallsTEvaluator::CallsTEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
}

bool CallsTEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isCallsT(leftLabel, rightLabel);
}

unordered_set<string> CallsTEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getAllProcCalledTBy(label);
}

unordered_set<string> CallsTEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getAllProcCalledTBy(label);
}

unordered_set<string> CallsTEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getAllProcCallingT(label);
}

vector<pair<string, string>> CallsTEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllCallsT();
}

unordered_set<string> CallsTEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllProcsCallingSomeProcs();
}

unordered_set<string> CallsTEvaluator::getWildCardIdentifierRelation(const string &label) {
    return pkb->getAllProcCallingT(label);
}

unordered_set<string> CallsTEvaluator::getWildCardSynonymRelation() {
    return pkb->getAllProcsCalledBySomeProcs();
}

vector<pair<string, string>> CallsTEvaluator::getWildCardWildCardRelation() {
    return pkb->getAllCallsT();
}
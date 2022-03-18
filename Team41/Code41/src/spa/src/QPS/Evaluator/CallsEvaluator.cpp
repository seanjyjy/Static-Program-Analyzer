#include "CallsEvaluator.h"

CallsEvaluator::CallsEvaluator(PKBClient *pkb): ProcProcEvaluator(pkb) {}

bool CallsEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isCalls(leftLabel, rightLabel);
}

unordered_set<string> CallsEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getAllProcCalledBy(label);
}

unordered_set<string> CallsEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getAllProcCalledBy(label);
}

unordered_set<string> CallsEvaluator::getSynonymIdentifierRelation(const string &label) {
    return  pkb->getAllProcCalling(label);
}

vector<pair<string, string>> CallsEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllCalls();
}

unordered_set<string> CallsEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllProcsCallingSomeProcs();
}

unordered_set<string> CallsEvaluator::getWildCardIdentifierRelation(const string &label) {
    return pkb->getAllProcCalling(label);
}

unordered_set<string> CallsEvaluator::getWildCardSynonymRelation() {
    return pkb->getAllProcsCalledBySomeProcs();
}

vector<pair<string, string>> CallsEvaluator::getWildCardWildCardRelation() {
    return pkb->getAllCalls();
}
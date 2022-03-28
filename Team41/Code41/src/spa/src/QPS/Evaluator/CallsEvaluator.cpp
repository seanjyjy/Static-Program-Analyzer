#include "CallsEvaluator.h"

CallsEvaluator::CallsEvaluator(PKBClient *pkb) : ProcProcEvaluator(pkb) {}

bool CallsEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isCalls(leftLabel, rightLabel);
}

unordered_set<string> CallsEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getAllProcCalledBy(label);
}

bool CallsEvaluator::getIdentifierWildCardRelation(const string &label) {
    unordered_set<string> allCallers = pkb->getAllProcsCallingSomeProcs();
    return allCallers.find(label) != allCallers.end();
}

unordered_set<string> CallsEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getAllProcCalling(label);
}

vector<pair<string, string>> CallsEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllCalls();
}

unordered_set<string> CallsEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllProcsCallingSomeProcs();
}

bool CallsEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allCallees = pkb->getAllProcsCalledBySomeProcs();
    return allCallees.find(label) != allCallees.end();
}

unordered_set<string> CallsEvaluator::getWildCardSynonymRelation() {
    return pkb->getAllProcsCalledBySomeProcs();
}

bool CallsEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllCalls().empty();
}
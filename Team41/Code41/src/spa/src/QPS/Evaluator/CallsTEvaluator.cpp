#include "CallsTEvaluator.h"

CallsTEvaluator::CallsTEvaluator(PKBClient *pkb) : ProcProcEvaluator(pkb) {}

bool CallsTEvaluator::getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isCallsT(leftLabel, rightLabel);
}

unordered_set<string> CallsTEvaluator::getIdentifierSynonymRelation(const string &label) {
    return pkb->getAllProcCalledTBy(label);
}

bool CallsTEvaluator::getIdentifierWildCardRelation(const string &label) {
    unordered_set<string> allCallers = pkb->getAllProcsCallingSomeProcs();
    return allCallers.find(label) != allCallers.end();
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

bool CallsTEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allCallees = pkb->getAllProcsCalledBySomeProcs();
    return allCallees.find(label) != allCallees.end();
}

unordered_set<string> CallsTEvaluator::getWildCardSynonymRelation() {
    return pkb->getAllProcsCalledBySomeProcs();
}

bool CallsTEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllCalls().empty();
}
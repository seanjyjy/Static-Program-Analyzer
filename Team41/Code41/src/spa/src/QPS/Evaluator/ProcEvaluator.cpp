#include "ProcEvaluator.h"

ProcEvaluator::ProcEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {}

Table *ProcEvaluator::evaluateIdentifierIdentifier(const ClauseVariable &left, const ClauseVariable &right) {
    bool isCallRelation = getIdentifierIdentifierRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(isCallRelation);
}

Table *ProcEvaluator::evaluateIdentifierSynonym(const ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> procCalledBy = getIdentifierSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(procCalledBy, right);
}

Table *ProcEvaluator::evaluateIdentifierWildCard(const ClauseVariable &left) {
    bool hasCallRelation = getIdentifierWildCardRelation(left.getLabel());
    return buildBooleanTable(hasCallRelation);
}

Table *ProcEvaluator::evaluateSynonymIdentifier(ClauseVariable &left, const ClauseVariable &right) {
    unordered_set<string> procCalling = getSynonymIdentifierRelation(right.getLabel());
    return buildSingleSynonymTable(procCalling, left);
}

Table *ProcEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    vector<pair<string, string>> listOfProcToProc = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(listOfProcToProc, left, right);
}

Table *ProcEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    unordered_set<string> listOfProcCalling = getSynonymWildCardRelation();
    return buildSingleSynonymTable(listOfProcCalling, left);
}

Table *ProcEvaluator::evaluateWildCardIdentifier(const ClauseVariable &right) {
    bool hasCallRelation = getWildCardIdentifierRelation(right.getLabel());
    return buildBooleanTable(hasCallRelation);
}

Table *ProcEvaluator::evaluateWildCardWildCard() {
    bool hasCallRelation = getWildCardWildCardRelation();
    return buildBooleanTable(hasCallRelation);
}

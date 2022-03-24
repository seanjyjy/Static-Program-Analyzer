#include "IfPatternEvaluator.h"
#include "Common/TreeUtils.h"

IfPatternEvaluator::IfPatternEvaluator(PKBClient *pkb) : IfWhilePatternEvaluator(pkb) {}

unordered_set<string> IfPatternEvaluator::getWildCardWildCardRelation() {
    return pkb->getIfStmtUsingSomeVarCond();
}

unordered_set<string> IfPatternEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getIfStmtUsingVarCond(label);
}

vector<pair<string, string>> IfPatternEvaluator::getSynonymWildCardRelation() {
    return pkb->getIfStmtNVarPairs();
}
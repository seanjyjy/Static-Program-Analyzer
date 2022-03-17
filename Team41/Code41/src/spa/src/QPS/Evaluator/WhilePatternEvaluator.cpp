#include "WhilePatternEvaluator.h"
#include "Common/TreeUtils.h"

WhilePatternEvaluator::WhilePatternEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
}

unordered_set<string> WhilePatternEvaluator::getWildCardWildCardRelation() {
    return pkb->getWhileStmtUsingSomeVarCond();
}

unordered_set<string> WhilePatternEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getWhileStmtUsingVarCond(label);
}

vector<pair<string, string>> WhilePatternEvaluator::getSynonymWildCardRelation() {
    return pkb->getWhileStmtNVarPairs();
}
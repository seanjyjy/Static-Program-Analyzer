#include "UsesSEvaluator.h"

UsesSEvaluator::UsesSEvaluator(PKBClient *pkb) : ModifiesUsesSEvaluator(pkb) {}

Table *UsesSEvaluator::evaluateClauseFurther(ClauseVariable &leftVariable, ClauseVariable &rightVariable) {
    if (EvaluatorUtils::SUtils::isValidUsesSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(leftVariable);
    }

    throw SemanticException("Invalid query provided for UsesS");
}

bool UsesSEvaluator::getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isUsesS(leftLabel, rightLabel);
}

unordered_set<string> UsesSEvaluator::getIntegerSynonymRelation(const string &label) {
    return pkb->getUsesByStmt(label);
}

unordered_set<string> UsesSEvaluator::getIntegerWildCardRelation(const string &label) {
    return pkb->getUsesByStmt(label);
}

unordered_set<string> UsesSEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getUsesSByVar(label);
}

vector<pair<string, string>> UsesSEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllUsesS();
}

unordered_set<string> UsesSEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllStmtsUsingSomeVar();
}

unordered_set<string> UsesSEvaluator::getWildCardIdentifierRelation(const string &label) {
    return pkb->getUsesSByVar(label);
}

vector<pair<string, string>> UsesSEvaluator::getWildCardWildCardRelation() {
    return pkb->getAllUsesS();
}

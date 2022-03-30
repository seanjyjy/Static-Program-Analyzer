#include "UsesSEvaluator.h"

UsesSEvaluator::UsesSEvaluator(PKBClient *pkb) : ModifiesUsesSEvaluator(pkb) {}

Table *UsesSEvaluator::evaluateClauseFurther(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (EvaluatorUtils::SUtils::isValidUsesSynonymIdentifier(&left, &right)) {
        return evaluateSynonymIdentifier(left, right, canSimplify);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(&left, &right)) {
        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(&left, &right)) {
        return evaluateSynonymWildCard(left, canSimplify);
    }

    throw SemanticException("Invalid query provided for UsesS");
}

bool UsesSEvaluator::getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isUsesS(leftLabel, rightLabel);
}

unordered_set<string> UsesSEvaluator::getIntegerSynonymRelation(const string &label) {
    return pkb->getUsesByStmt(label);
}

bool UsesSEvaluator::getIntegerWildCardRelation(const string &label) {
    unordered_set<string> allStmt = pkb->getAllStmtsUsingSomeVar();
    return allStmt.find(label) != allStmt.end();
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

bool UsesSEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allVar = pkb->getAllVarUsedBySomeStmt();
    return allVar.find(label) != allVar.end();
}

bool UsesSEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllUsesS().empty();
}

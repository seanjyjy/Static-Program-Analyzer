#include "ModifiesSEvaluator.h"

ModifiesSEvaluator::ModifiesSEvaluator(PKBClient *pkb) : ModifiesUsesSEvaluator(pkb) {}

Table *ModifiesSEvaluator::evaluateClauseFurther(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (EvaluatorUtils::SUtils::isValidModifiesSynonymIdentifier(&left, &right)) {
        return evaluateSynonymIdentifier(left, right, canSimplify);
    }

    if (EvaluatorUtils::SUtils::isValidModifiesSynonymSynonym(&left, &right)) {
        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::SUtils::isValidModifiesSynonymWildCard(&left, &right)) {
        return evaluateSynonymWildCard(left, canSimplify);
    }

    throw SemanticException("Invalid query provided for ModifiesS");
}

bool ModifiesSEvaluator::getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) {
    return pkb->isModifiesS(leftLabel, rightLabel);
}

unordered_set<string> ModifiesSEvaluator::getIntegerSynonymRelation(const string &label) {
    return pkb->getModifiesByStmt(label);
}

bool ModifiesSEvaluator::getIntegerWildCardRelation(const string &label) {
    unordered_set<string> allStmt = pkb->getAllStmtsModifyingSomeVar();
    return allStmt.find(label) != allStmt.end();
}

unordered_set<string> ModifiesSEvaluator::getSynonymIdentifierRelation(const string &label) {
    return pkb->getModifiesSByVar(label);
}

vector<pair<string, string>> ModifiesSEvaluator::getSynonymSynonymRelation() {
    return pkb->getAllModifiesS();
}

unordered_set<string> ModifiesSEvaluator::getSynonymWildCardRelation() {
    return pkb->getAllStmtsModifyingSomeVar();
}

bool ModifiesSEvaluator::getWildCardIdentifierRelation(const string &label) {
    unordered_set<string> allVar = pkb->getAllVarModifiedBySomeStmt();
    return allVar.find(label) != allVar.end();
}

bool ModifiesSEvaluator::getWildCardWildCardRelation() {
    return !pkb->getAllModifiesS().empty();
}

#include "ModifiesSEvaluator.h"

ModifiesSEvaluator::ModifiesSEvaluator(PKBClient *pkb) : ModifiesUsesSEvaluator(pkb) {}

Table *ModifiesSEvaluator::evaluateClauseFurther(ClauseVariable &leftVariable, ClauseVariable &rightVariable) {
    if (EvaluatorUtils::SUtils::isValidModifiesSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidModifiesSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidModifiesSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(leftVariable);
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

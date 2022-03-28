#include "ModifiesUsesSEvaluator.h"

ModifiesUsesSEvaluator::ModifiesUsesSEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {}

Table *ModifiesUsesSEvaluator::evaluateClause(ClauseVariable &leftVariable, ClauseVariable &rightVariable) {
    if (EvaluatorUtils::SUtils::isIntegerIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIntegerIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isValidIntegerSynonym(&leftVariable, &rightVariable)) {
        return evaluateIntegerSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::SUtils::isIntegerWildCard(&leftVariable, &rightVariable)) {
        return evaluateIntegerWildCard(leftVariable);
    }

    if (EvaluatorUtils::isWildCardIdentifier(&leftVariable, &rightVariable)) {
        return evaluateWildCardIdentifier(rightVariable);
    }

    if (EvaluatorUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard();
    }

    return evaluateClauseFurther(leftVariable, rightVariable);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerIdentifier(const ClauseVariable &left, const ClauseVariable &right) {
    bool isStatementUsed = getIntegerIdentifierRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(isStatementUsed);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerSynonym(const ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> statementUsed = getIntegerSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(statementUsed, right);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerWildCard(const ClauseVariable &left) {
    unordered_set<string> statementUsed = getIntegerWildCardRelation(left.getLabel());
    return buildBooleanTable(!statementUsed.empty());
}

Table *ModifiesUsesSEvaluator::evaluateSynonymIdentifier(ClauseVariable &left, ClauseVariable &right) {
    unordered_set<string> statementBeingUsed = getSynonymIdentifierRelation(right.getLabel());
    return buildSingleSynonymTable(statementBeingUsed, left);
}

Table *ModifiesUsesSEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right) {
    vector<pair<string, string>> listOfStmtStmt = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(listOfStmtStmt, left, right);
}

Table *ModifiesUsesSEvaluator::evaluateSynonymWildCard(ClauseVariable &left) {
    unordered_set<string> statementUsed = getSynonymWildCardRelation();
    return buildSingleSynonymTable(statementUsed, left);
}

Table *ModifiesUsesSEvaluator::evaluateWildCardIdentifier(ClauseVariable &right) {
    unordered_set<string> statementBeingUsed = getWildCardIdentifierRelation(right.getLabel());
    return buildBooleanTable(!statementBeingUsed.empty());
}

Table *ModifiesUsesSEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> statementUsed = getWildCardWildCardRelation();
    return buildBooleanTable(!statementUsed.empty());
}

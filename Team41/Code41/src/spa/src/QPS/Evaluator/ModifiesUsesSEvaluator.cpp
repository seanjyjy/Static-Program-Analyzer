#include "ModifiesUsesSEvaluator.h"

ModifiesUsesSEvaluator::ModifiesUsesSEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {}

Table *ModifiesUsesSEvaluator::evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (EvaluatorUtils::SUtils::isIntegerIdentifier(&left, &right)) {
        return evaluateIntegerIdentifier(left, right);
    }

    if (EvaluatorUtils::SUtils::isValidIntegerSynonym(&left, &right)) {
        return evaluateIntegerSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::SUtils::isIntegerWildCard(&left, &right)) {
        return evaluateIntegerWildCard(left);
    }

    if (EvaluatorUtils::isWildCardIdentifier(&left, &right)) {
        return evaluateWildCardIdentifier(right);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    return evaluateClauseFurther(left, right, canSimplify);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerIdentifier(const ClauseVariable &left, const ClauseVariable &right) {
    bool isStatementUsed = getIntegerIdentifierRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(isStatementUsed);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerSynonym(const ClauseVariable &left, ClauseVariable &right,
                                                      bool canSimplify) {
    unordered_set<string> statementUsed = getIntegerSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(statementUsed, right, canSimplify);
}

Table *ModifiesUsesSEvaluator::evaluateIntegerWildCard(const ClauseVariable &left) {
    bool isStatementUsed = getIntegerWildCardRelation(left.getLabel());
    return buildBooleanTable(isStatementUsed);
}

Table *ModifiesUsesSEvaluator::evaluateSynonymIdentifier(ClauseVariable &left, ClauseVariable &right,
                                                         bool canSimplify) {
    unordered_set<string> statementBeingUsed = getSynonymIdentifierRelation(right.getLabel());
    return buildSingleSynonymTable(statementBeingUsed, left, canSimplify);
}

Table *ModifiesUsesSEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<pair<string, string>> listOfStmtStmt = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(listOfStmtStmt, left, right, canSimplify);
}

Table *ModifiesUsesSEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    unordered_set<string> statementUsed = getSynonymWildCardRelation();
    return buildSingleSynonymTable(statementUsed, left, canSimplify);
}

Table *ModifiesUsesSEvaluator::evaluateWildCardIdentifier(ClauseVariable &right) {
    bool isStatementBeingUsed = getWildCardIdentifierRelation(right.getLabel());
    return buildBooleanTable(isStatementBeingUsed);
}

Table *ModifiesUsesSEvaluator::evaluateWildCardWildCard() {
    bool hasRelation = getWildCardWildCardRelation();
    return buildBooleanTable(hasRelation);
}

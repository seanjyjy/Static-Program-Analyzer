#include "StmtStmtEvaluator.h"

StmtStmtEvaluator::StmtStmtEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {}

Table *StmtStmtEvaluator::evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&left, &right)) {
        return evaluateIntegerInteger(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&left, &right)) {
        return evaluateIntegerSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&left, &right)) {
        return evaluateIntegerWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&left, &right)) {
        return evaluateSynonymInteger(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&left, &right)) {
        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&left, &right)) {
        return evaluateSynonymWildCard(left, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&left, &right)) {
        return evaluateWildCardInteger(right);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&left, &right)) {
        return evaluateWildCardSynonym(right, canSimplify);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided");
}
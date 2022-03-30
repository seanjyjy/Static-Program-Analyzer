#include "ProcProcEvaluator.h"

ProcProcEvaluator::ProcProcEvaluator(PKBClient *pkb) : ProcEvaluator(pkb) {}

Table *ProcProcEvaluator::evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&left, &right)) {
        return evaluateIdentifierIdentifier(left, right);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsIdentifierSynonym(&left, &right)) {
        return evaluateIdentifierSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&left, &right)) {
        return evaluateIdentifierWildCard(left);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymIdentifier(&left, &right)) {
        return evaluateSynonymIdentifier(left, right, canSimplify);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymSynonym(&left, &right)) {
        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymWildCard(&left, &right)) {
        return evaluateSynonymWildCard(left, canSimplify);
    }

    if (EvaluatorUtils::isWildCardIdentifier(&left, &right)) {
        return evaluateWildCardIdentifier(right);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsWildCardSynonym(&left, &right)) {
        return evaluateWildCardSynonym(right, canSimplify);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided for Call");
}

Table *ProcProcEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    unordered_set<string> listOfProcBeingCalled = getWildCardSynonymRelation();
    return buildSingleSynonymTable(listOfProcBeingCalled, right, canSimplify);
}
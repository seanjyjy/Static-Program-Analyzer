#include "ModifiesUsesPEvaluator.h"

ModifiesUsesPEvaluator::ModifiesUsesPEvaluator(PKBClient *pkb) : ProcEvaluator(pkb) {}

Table *ModifiesUsesPEvaluator::evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&left, &right)) {
        return evaluateIdentifierIdentifier(left, right);
    }

    if (EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(&left, &right)) {
        return evaluateIdentifierSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&left, &right)) {
        return evaluateIdentifierWildCard(left);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(&left, &right)) {
        return evaluateSynonymIdentifier(left, right, canSimplify);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymSynonym(&left, &right)) {
        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymWildCard(&left, &right)) {
        return evaluateSynonymWildCard(left, canSimplify);
    }

    throw SemanticException("Invalid query provided for ModifiesP or UsesP");
}

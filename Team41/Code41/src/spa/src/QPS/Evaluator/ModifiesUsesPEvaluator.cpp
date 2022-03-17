#include "ModifiesUsesPEvaluator.h"

Table *ModifiesUsesPEvaluator::evaluateClause(ClauseVariable leftVariable, ClauseVariable rightVariable) {

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIdentifierIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(&leftVariable, &rightVariable)) {
        return evaluateIdentifierSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&leftVariable, &rightVariable)) {
        return evaluateIdentifierWildCard(leftVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(leftVariable);
    }

    throw SemanticException("Invalid query provided for ModifiesP or UsesP");
}

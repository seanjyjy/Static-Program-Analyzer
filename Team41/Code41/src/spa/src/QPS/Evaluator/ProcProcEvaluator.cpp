#include "ProcProcEvaluator.h"

ProcProcEvaluator::ProcProcEvaluator(PKBClient *pkb) : ProcEvaluator(pkb) {}

Table *ProcProcEvaluator::evaluateClause(ClauseVariable leftVariable, ClauseVariable rightVariable) {

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIdentifierIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsIdentifierSynonym(&leftVariable, &rightVariable)) {
        return evaluateIdentifierSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&leftVariable, &rightVariable)) {
        return evaluateIdentifierWildCard(leftVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(leftVariable);
    }

    if (EvaluatorUtils::CallsUtils::isWildCardIdentifier(&leftVariable, &rightVariable)) {
        return evaluateWildCardIdentifier(rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsWildCardSynonym(&leftVariable, &rightVariable)) {
        return evaluateWildCardSynonym(rightVariable);
    }

    if (EvaluatorUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided for Call");
}

Table *ProcProcEvaluator::evaluateWildCardIdentifier(const ClauseVariable& right) {
    unordered_set<string> procCalling = getWildCardIdentifierRelation(right.getLabel());
    return buildBooleanTable(!procCalling.empty());
}

Table *ProcProcEvaluator::evaluateWildCardSynonym(ClauseVariable& right) {
    unordered_set<string> listOfProcBeingCalled = getWildCardSynonymRelation();
    return buildSingleSynonymTable(listOfProcBeingCalled, right);
}

Table *ProcProcEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfProcToProc = getWildCardWildCardRelation();
    return buildBooleanTable(!listOfProcToProc.empty());
}
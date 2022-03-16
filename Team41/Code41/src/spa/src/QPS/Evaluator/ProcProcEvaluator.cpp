#include "ProcProcEvaluator.h"

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

Table *ProcProcEvaluator::evaluateIdentifierIdentifier(ClauseVariable left, ClauseVariable right) {
    bool isCallRelation = getIdentifierIdentifierRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(isCallRelation);
}

Table *ProcProcEvaluator::evaluateIdentifierSynonym(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> procCalledBy = getIdentifierSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(procCalledBy, right);
}

Table *ProcProcEvaluator::evaluateIdentifierWildCard(ClauseVariable left) {
    unordered_set<string> procCalledBy = getIdentifierWildCardRelation(left.getLabel());
    return buildBooleanTable(procCalledBy);
}

Table *ProcProcEvaluator::evaluateSynonymIdentifier(ClauseVariable left, ClauseVariable right) {
    unordered_set<string> procCalling = getSynonymIdentifierRelation(right.getLabel());
    return buildSingleSynonymTable(procCalling, left);
}

Table *ProcProcEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfProcToProc = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(listOfProcToProc, left, right);
}

Table *ProcProcEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    unordered_set<string> listOfProcCalling = getSynonymWildCardRelation();
    return buildSingleSynonymTable(listOfProcCalling, left);
}

Table *ProcProcEvaluator::evaluateWildCardIdentifier(ClauseVariable right) {
    unordered_set<string> procCalling = getWildCardIdentifierRelation(right.getLabel());
    return buildBooleanTable(procCalling);
}

Table *ProcProcEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    unordered_set<string> listOfProcBeingCalled = getWildCardSynonymRelation();
    return buildSingleSynonymTable(listOfProcBeingCalled, right);
}

Table *ProcProcEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> listOfProcToProc = getWildCardWildCardRelation();
    return buildBooleanTable(listOfProcToProc);
}

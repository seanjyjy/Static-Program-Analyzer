#include "IfWhilePatternEvaluator.h"

IfWhilePatternEvaluator::IfWhilePatternEvaluator(PKBClient *pkb) : PatternEvaluator(pkb) {}

Table *IfWhilePatternEvaluator::evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable &leftVariable,
                                                vector<PatternVariable> &rightPatternVariables) {

    if (EvaluatorUtils::PatternUtils::isValidSynonymWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateSynonymWildCard(patternSynonym, leftVariable);
    }

    if (EvaluatorUtils::PatternUtils::isIdentifierWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateIdentifierWildCard(patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *IfWhilePatternEvaluator::evaluateSynonymWildCard(QueryDeclaration& patternSyn, ClauseVariable& left) {
    vector<pair<string, string>> listOfStmtNVar = getSynonymWildCardRelation();
    return buildSynonymSynonymPatternTable(listOfStmtNVar, patternSyn, left);
}

Table *IfWhilePatternEvaluator::evaluateIdentifierWildCard(QueryDeclaration patternSynonym,
                                                           const ClauseVariable &leftVariable) {
    unordered_set<string> setOfPatternStmt = getIdentifierWildCardRelation(leftVariable.getLabel());
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}


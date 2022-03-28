#include "IfWhilePatternEvaluator.h"

IfWhilePatternEvaluator::IfWhilePatternEvaluator(PKBClient *pkb) : PatternEvaluator(pkb) {}

Table *IfWhilePatternEvaluator::evaluateFurther(QueryDeclaration &patternSynonym, ClauseVariable &leftVariable,
                                                vector<PatternVariable> &rightPatternVariables, bool canSimplify) {

    if (EvaluatorUtils::PatternUtils::isValidSynonymWildCards(leftVariable, rightPatternVariables,
                                                              patternSynonym.type)) {
        return evaluateSynonymWildCard(patternSynonym, leftVariable, canSimplify);
    }

    if (EvaluatorUtils::PatternUtils::isIdentifierWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateIdentifierWildCard(patternSynonym, leftVariable, canSimplify);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *IfWhilePatternEvaluator::evaluateSynonymWildCard(QueryDeclaration &patternSyn, ClauseVariable &left, bool canSimplify) {
    vector<pair<string, string>> listOfStmtNVar = getSynonymWildCardRelation();
    if (canSimplify) {
        return buildBooleanTable(!listOfStmtNVar.empty());
    }
    return buildSynonymSynonymPatternTable(listOfStmtNVar, patternSyn, left);
}

Table *IfWhilePatternEvaluator::evaluateIdentifierWildCard(QueryDeclaration &patternSynonym,
                                                           const ClauseVariable &leftVariable, bool canSimplify) {
    unordered_set<string> setOfPatternStmt = getIdentifierWildCardRelation(leftVariable.getLabel());
    if (canSimplify) {
        return buildBooleanTable(!setOfPatternStmt.empty());
    }
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}


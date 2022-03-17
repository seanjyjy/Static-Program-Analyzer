#include "IfWhilePatternEvaluator.h"

Table *IfWhilePatternEvaluator::evaluateFurther(QueryDeclaration patternSynonym, const ClauseVariable &leftVariable,
                                                vector<PatternVariable> &rightPatternVariables) {

    if (EvaluatorUtils::PatternUtils::isValidSynonymWildCards(leftVariable, rightPatternVariables)) {
        return evaluateSynonymWildCard(patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *IfWhilePatternEvaluator::evaluateSynonymWildCard(QueryDeclaration patternSyn, ClauseVariable left) {
    vector<pair<string, string>> listOfStmtNVar = getSynonymWildCardRelation();
    return buildSynonymSynonymTable(listOfStmtNVar, patternSyn, left);
}

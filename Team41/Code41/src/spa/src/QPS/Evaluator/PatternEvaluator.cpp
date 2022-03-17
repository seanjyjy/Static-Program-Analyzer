#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *PatternEvaluator::evaluate(PatternClause clause) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    vector<PatternVariable> rightPatternVariables = clause.getRHS();

    if (leftVariable.getLabel() == patternSynonym.synonym) {
        throw SemanticException("NO SUCH COMBINATION ALLOWED FOR PATTERN");
    }

    if (EvaluatorUtils::PatternUtils::isWildCardWildCards(leftVariable, rightPatternVariables)) {
        return evaluateWildCardWildCard(patternSynonym);
    }

    if (EvaluatorUtils::PatternUtils::isIdentifierWildCards(leftVariable, rightPatternVariables)) {
        return evaluateIdentifierWildCard(patternSynonym, leftVariable);
    }

    return evaluateFurther(patternSynonym, leftVariable, rightPatternVariables);
}

Table *PatternEvaluator::evaluateWildCardWildCard(QueryDeclaration patternSynonym) {
    unordered_set<string> setOfPatternStmt = getWildCardWildCardRelation();
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}

Table *PatternEvaluator::evaluateIdentifierWildCard(QueryDeclaration patternSynonym, const ClauseVariable& leftVariable) {
    unordered_set<string> setOfPatternStmt = getIdentifierWildCardRelation(leftVariable.getLabel());
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}

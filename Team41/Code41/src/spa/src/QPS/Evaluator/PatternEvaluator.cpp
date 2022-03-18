#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *PatternEvaluator::evaluate(PatternClause clause) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    vector<PatternVariable> rightPatternVariables = clause.getRHS();

    if (leftVariable.getLabel() == patternSynonym.synonym) {
        throw SemanticException("NO SUCH COMBINATION ALLOWED FOR PATTERN");
    }

    if (EvaluatorUtils::PatternUtils::isWildCardWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateWildCardWildCard(patternSynonym);
    }

    return evaluateFurther(patternSynonym, leftVariable, rightPatternVariables);
}

Table *PatternEvaluator::evaluate(QueryClause clause) {
    throw runtime_error("Pattern clause should NEVER COME HERE");
}

Table *PatternEvaluator::evaluateWildCardWildCard(QueryDeclaration patternSynonym) {
    unordered_set<string> setOfPatternStmt = getWildCardWildCardRelation();
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}
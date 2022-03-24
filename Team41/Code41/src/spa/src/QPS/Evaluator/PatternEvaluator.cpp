#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *PatternEvaluator::evaluate(const PatternClause &clause) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    vector<PatternVariable> rightPatternVariables = clause.getRHS();

    if (leftVariable.getLabel() == patternSynonym.getSynonym()) {
        throw SemanticException("NO SUCH COMBINATION ALLOWED FOR PATTERN");
    }

    if (EvaluatorUtils::PatternUtils::isWildCardWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateWildCardWildCard(patternSynonym);
    }

    return evaluateFurther(patternSynonym, leftVariable, rightPatternVariables);
}

Table *PatternEvaluator::evaluateWildCardWildCard(QueryDeclaration patternSynonym) {
    unordered_set<string> setOfPatternStmt = getWildCardWildCardRelation();
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}
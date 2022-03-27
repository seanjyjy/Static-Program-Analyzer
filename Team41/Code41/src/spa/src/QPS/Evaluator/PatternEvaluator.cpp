#include "PatternEvaluator.h"

PatternEvaluator::PatternEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *PatternEvaluator::evaluate(const PatternClause &clause) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    vector<PatternVariable> rightPatternVariables = clause.getRHS();

    if (EvaluatorUtils::PatternUtils::isWildCardWildCards(leftVariable, rightPatternVariables, patternSynonym.type)) {
        return evaluateWildCardWildCard(patternSynonym, clause.canSimplifyClause());
    }

    return evaluateFurther(patternSynonym, leftVariable, rightPatternVariables, clause.canSimplifyClause());
}

Table *PatternEvaluator::evaluateWildCardWildCard(QueryDeclaration patternSynonym, bool canSimplify) {
    unordered_set<string> setOfPatternStmt = getWildCardWildCardRelation();
    if (canSimplify) {
        return buildBooleanTable(!setOfPatternStmt.empty());
    }
    return buildSingleSynonymTable(setOfPatternStmt, patternSynonym);
}
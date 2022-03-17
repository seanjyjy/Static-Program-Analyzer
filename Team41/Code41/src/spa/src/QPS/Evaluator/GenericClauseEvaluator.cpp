#include "GenericClauseEvaluator.h"

GenericClauseEvaluator::GenericClauseEvaluator(PKBClient *pkb) : Evaluator(pkb) {}
GenericClauseEvaluator::GenericClauseEvaluator(PKBClient *pkb, NextKBAdapter* nextKBAdapter) : Evaluator(pkb, nextKBAdapter) {}

Table *GenericClauseEvaluator::evaluate(QueryClause clause) {
    ClauseVariable leftVariable = clause.getLeftClauseVariable();
    ClauseVariable rightVariable = clause.getRightClauseVariable();
    return evaluateClause(leftVariable, rightVariable);
}

Table *GenericClauseEvaluator::evaluate(PatternClause clause) {
    throw runtime_error("Pattern clause should NEVER COME HERE");
}

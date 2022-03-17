#include "GenericClauseEvaluator.h"

GenericClauseEvaluator::GenericClauseEvaluator(PKBClient *pkb) : Evaluator(pkb) {}
GenericClauseEvaluator::GenericClauseEvaluator(NextKBAdapter* nextKBAdapter) : Evaluator(nextKBAdapter) {}

Table *GenericClauseEvaluator::evaluate(QueryClause clause) {
    ClauseVariable leftVariable = clause.getLeftClauseVariable();
    ClauseVariable rightVariable = clause.getRightClauseVariable();
    return evaluateClause(leftVariable, rightVariable);
}

Table *GenericClauseEvaluator::evaluate(PatternClause clause) {
    throw runtime_error("Pattern clause should NEVER COME HERE");
}

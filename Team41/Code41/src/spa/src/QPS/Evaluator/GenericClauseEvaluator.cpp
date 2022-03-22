#include "GenericClauseEvaluator.h"

GenericClauseEvaluator::GenericClauseEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *GenericClauseEvaluator::evaluate(QueryClause *clause) {
    ClauseVariable leftVariable = clause->getLeftClauseVariable();
    ClauseVariable rightVariable = clause->getRightClauseVariable();
    return evaluateClause(leftVariable, rightVariable);
}
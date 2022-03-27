#include "GenericClauseEvaluator.h"

GenericClauseEvaluator::GenericClauseEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

Table *GenericClauseEvaluator::evaluate(const QueryClause& clause) {
    QueryClause clauseToUse = clause;
    if (clause.canSimplifyClause() && !clause.isBooleanClause())
        clauseToUse = clause.generateSimplifiedSelf();

    ClauseVariable leftVariable = clauseToUse.getLeftClauseVariable();
    ClauseVariable rightVariable = clauseToUse.getRightClauseVariable();
    return evaluateClause(leftVariable, rightVariable);
}
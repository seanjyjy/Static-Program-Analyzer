#include "GenericClauseEvaluator.h"

Table *GenericClauseEvaluator::evaluate(QueryClause clause) {
    ClauseVariable leftVariable = clause.getLeftClauseVariable();
    ClauseVariable rightVariable = clause.getRightClauseVariable();
    return evaluateClause(leftVariable, rightVariable);
}

#include "QueryClause.h"

QueryClause::QueryClause(clause_type type, ClauseVariable &left, ClauseVariable &right) :
    type(type), left(left), right(right) {}

ClauseVariable & QueryClause::getLeftClauseVariable() const {
    return left;
}

ClauseVariable &QueryClause::getRightClauseVariable() const {
    return right;
}

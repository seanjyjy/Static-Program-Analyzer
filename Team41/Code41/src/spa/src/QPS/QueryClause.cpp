#include "QueryClause.h"

#include <utility>

QueryClause::QueryClause(clause_type type, ClauseVariable left, ClauseVariable right) :
    type(type), left(std::move(left)), right(std::move(right)) {}

ClauseVariable QueryClause::getLeftClauseVariable() const {
    return left;
}

ClauseVariable QueryClause::getRightClauseVariable() const {
    return right;
}

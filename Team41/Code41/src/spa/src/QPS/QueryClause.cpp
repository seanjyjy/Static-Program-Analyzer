#include "QueryClause.h"

QueryClause::QueryClause(clause_type type, ClauseVariable left, ClauseVariable right) :
    type(type), left(left), right(right) {}

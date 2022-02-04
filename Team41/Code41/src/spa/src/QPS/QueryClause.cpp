#include "QueryClause.h"

QueryClause::QueryClause(clause_type type, QueryDeclaration left, QueryDeclaration right) :
    type(type), left(left), right(right) {}

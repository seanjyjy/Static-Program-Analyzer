#include "QueryObject.h"

QueryObject::QueryObject(vector<QueryDeclaration> &declarations, vector<QueryClause> &clauses, bool isQueryValid) :
declarations(declarations), clauses(clauses), isQueryValid(isQueryValid) {}

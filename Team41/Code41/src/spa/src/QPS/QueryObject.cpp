#include "QueryObject.h"

QueryObject::QueryObject(vector<QueryDeclaration> &declarations, vector<QueryClause> &clauses, QueryDeclaration &selectSynonym, bool isQueryValid) :
declarations(declarations), clauses(clauses), selectSynonym(selectSynonym), isQueryValid(isQueryValid) {}

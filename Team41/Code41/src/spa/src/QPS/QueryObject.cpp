#include "QueryObject.h"

#include <utility>

QueryObject::QueryObject(vector<QueryDeclaration> declarations,
                         vector<QueryClause> clauses,
                         QueryDeclaration selectSynonym,
                         bool isQueryValid) :
    declarations(std::move(declarations)),
    clauses(std::move(clauses)),
    selectSynonym(std::move(selectSynonym)),
    isQueryValid(isQueryValid) {}

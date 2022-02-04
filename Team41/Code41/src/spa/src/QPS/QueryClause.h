#pragma once

#include "QueryDeclaration.h"

using namespace std;

class QueryClause {
public:
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, pattern
    };
    clause_type type;
    QueryDeclaration left, right; // Have yet to handle wild type '_'

    QueryClause(clause_type type, QueryDeclaration left, QueryDeclaration right);
};

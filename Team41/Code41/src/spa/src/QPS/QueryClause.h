#pragma once

#include "QueryDeclaration.h"
#include "ClauseVariable.h"

using namespace std;

class QueryClause {
public:
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, pattern
    };
    clause_type type;
    ClauseVariable left, right; // Have yet to handle wild type '_'

    QueryClause(clause_type type, ClauseVariable left, ClauseVariable right);
};
// todo: Add a ClauseVariable class. Use it for firstVariable(left), and secondVariable(right)

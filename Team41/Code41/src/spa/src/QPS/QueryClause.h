#pragma once

#include "QueryDeclaration.h"
#include "ClauseVariable.h"

using namespace std;

class QueryClause {
private:
    ClauseVariable left, right;
public:
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, pattern
    };
    clause_type type;

    ClauseVariable getLeftClauseVariable() const;
    ClauseVariable getRightClauseVariable() const;
    QueryClause(clause_type type, ClauseVariable left, ClauseVariable right);
};

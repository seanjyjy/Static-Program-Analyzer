#pragma once

#include "QueryDeclaration.h"
#include "ClauseVariable.h"

using namespace std;

class QueryClause {
private:
    // The LHS and RHS of a clause in PQL
    ClauseVariable left, right;
public:
    // enum representing possible clause types in PQL
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, calls, callsT, next, nextT, affects, affectsT
    };

    // clause type of the clause instance
    clause_type type;

    /**
     * Getter for the LHS.
     *
     * @return ClauseVariable representing the LHS.
     */
    ClauseVariable getLeftClauseVariable() const;

    /**
     * Getter for the RHS.
     *
     * @return ClauseVariable representing the RHS.
     */
    ClauseVariable getRightClauseVariable() const;

    /**
     * Constructor for the QueryClause class.
     *
     * @param type of the new clause.
     * @param left for the LHS ClauseVariable instance.
     * @param right for the RHS ClauseVariable instance.
     */
    QueryClause(clause_type type, ClauseVariable left, ClauseVariable right);
};

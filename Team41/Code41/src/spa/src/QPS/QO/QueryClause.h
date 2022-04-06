#pragma once

#include "QueryDeclaration.h"
#include "ClauseVariable.h"
#include "QPS/SimplifiableClause.h"
#include <vector>

using namespace std;

class QueryClause : public SimplifiableClause {
private:
    // The LHS and RHS of a clause in PQL
    ClauseVariable left, right;
public:
    // enum representing possible clause types in PQL
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, calls, callsT, next, nextT, affects, affectsT,
        generic_uses, generic_modifies, none
    };

    // todo: make private after all refactor
    // clause type of the clause instance
    clause_type type;

    clause_type getType() const;

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
     * Returns a human-readable string representation of the query clause.
     */
    string toString() const;

    /**
     * Constructor for the QueryClause class.
     *
     * @param type of the new clause.
     * @param left for the LHS ClauseVariable instance.
     * @param right for the RHS ClauseVariable instance.
     */
    QueryClause(clause_type type, ClauseVariable left, ClauseVariable right);

    QueryClause();

    bool hasSynonyms() const;

    vector<QueryDeclaration> getSynonyms() const;

    int hash() const;

    bool equals(QueryClause) const;
};

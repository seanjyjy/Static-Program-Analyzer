#pragma once

#include "QueryDeclaration.h"
#include "ClauseVariable.h"
#include <vector>

using namespace std;

class QueryClause {
private:
    // The LHS and RHS of a clause in PQL
    ClauseVariable left, right;
    bool canSimplify;
public:
    // enum representing possible clause types in PQL
    enum clause_type {
        follows, followsT, parent, parentT, usesS, usesP, modifiesS, modifiesP, calls, callsT, next, nextT, affects, affectsT, none
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
     * Converts the clause type enum into a human-readable string.
     *
     * @param c the valid clause type enum.
     * @return a human-readable string representation.
     * @throws runtime_error if the given clause type has no string mapping.
     */
    string typeToString() const;

    /**
     * Returns a human-readable string representation of the query clause.
     */
    string toString() const;

    /**
     * Prints the query clause's type and clause variables in a human-readable form.
     */
    void print();

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

    QueryClause generateSimplifiedSelf() const;

    bool canSimplifyClause() const;

    void setSimplified(bool canSimplifyClause);

    bool isBooleanClause() const;
};

#pragma once

#include "WithVariable.h"
#include "QPS/SimplifiableClause.h"
#include <vector>

using namespace std;

class WithClause : public SimplifiableClause {
private:
    WithVariable left, right;
public:
    WithClause();

    WithClause(WithVariable, WithVariable);

    /**
     * Getter for the left side in with clause.
     *
     * @return WithVariable representing the LHS.
     */
    WithVariable getLeft() const;

    /**
     * Getter for the right side in with clause.
     *
     * @return WithVariable representing the RHS.
     */
    WithVariable getRight() const;

    /**
     * Getter for the synonyms in With clause.
     *
     * @return vector of QueryDeclarations
     */
    vector<QueryDeclaration> getSynonyms() const;

    /**
     * Checks if with clause has synonyms.
     *
     * @return boolean indicating if it has synonyms.
     */
    bool hasSynonyms() const;

    /**
     * Hash function for the with clause.
     *
     * @return int for the hash.
     */
    int hash() const;

    /**
     * Equivalence comparator for the WithClause class.
     *
     * @return boolean indicating if equivalent.
     */
    bool equals(WithClause) const;

    /**
     * Gets a string representation of the with clause.
     *
     * @return string representation.
     */
    string toString() const;
};

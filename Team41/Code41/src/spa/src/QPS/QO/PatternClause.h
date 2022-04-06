#pragma once

#include <vector>
#include "Common/AstNode/TNode.h"
#include "ClauseVariable.h"
#include "QueryDeclaration.h"
#include "PatternVariable.h"
#include "QPS/SimplifiableClause.h"

class PatternClause : public SimplifiableClause {
private:
    // the synonym attached to the pattern clause
    QueryDeclaration synonym;

    // LHS of the pattern clause
    ClauseVariable lhs;

    // RHS of the pattern clause
    vector<PatternVariable> rhs; // miniAST exists within

public:
    /**
     * Getter for the synonym.
     *
     * @return QueryDeclaration representing the synonym of the pattern.
     */
    QueryDeclaration getSynonym() const;

    /**
     * Getter for the LHS.
     *
     * @return ClauseVariable representing the LHS of the pattern.
     */
    ClauseVariable getLHS() const;

    /**
     * Getter for the RHS.
     *
     * @return PatternVariable representing the RHS of the pattern.
     */
    vector<PatternVariable> getRHS() const;

    /**
     * Constructor for the PatternClause class.
     *
     * @param synonym of the pattern clause
     * @param left for the LHS ClauseVariable instance.
     * @param right for the RHS PatternVariable instance.
     */
    PatternClause(QueryDeclaration synonym, ClauseVariable lhs, vector<PatternVariable> rhs);

    PatternClause();

    /**
     * Checks if pattern clause has synonyms.
     *
     * @return boolean indicating if it has synonyms.
     */
    bool hasSynonyms() const;

    /**
     * Getter for the synonyms in pattern clause.
     *
     * @return vector of QueryDeclarations
     */
    vector<QueryDeclaration> getSynonyms() const;

    /**
     * Equivalence comparator for the PatternClause class.
     *
     * @return boolean indicating if equivalent.
     */
    bool equals(PatternClause) const;

    /**
     * Gets a string representation of the pattern clause.
     *
     * @return string representation.
     */
    string toString() const;

    /**
     * Hash function for the pattern clause.
     *
     * @return int for the hash.
     */
    int hash() const;

    void cleanUp();
};

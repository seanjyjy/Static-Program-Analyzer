#pragma once

#include <vector>
#include "Common/TNode.h"
#include "ClauseVariable.h"
#include "QueryDeclaration.h"
#include "PatternVariable.h"
#include "SimplifiableClause.h"

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

    bool hasSynonyms() const;

    vector<QueryDeclaration> getSynonyms() const;

    bool equals(PatternClause) const;

    string toString() const;

    int hash() const;
};

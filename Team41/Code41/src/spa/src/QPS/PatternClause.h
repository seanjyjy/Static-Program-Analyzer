#pragma once

#include <vector>
#include "Common/TNode.h"
#include "ClauseVariable.h"
#include "QueryDeclaration.h"
#include "PatternVariable.h"
#include "SuperClause.h"

class PatternClause : public SuperClause {
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

    // For SuperClause
    vector<QueryDeclaration> getSynonyms();
    bool hasSynonyms() const;
    bool isWithClause() const;
    bool isSuchThatClause() const;
    bool isPatternClause() const;
    bool isFollows() const;
    bool isFollowsT() const;
    bool isParent() const;
    bool isParentT() const;
    bool isUsesS() const;
    bool isUsesP() const;
    bool isModifiesS() const;
    bool isModifiesP() const;
    bool isCalls() const;
    bool isCallsT() const;
    bool isNext() const;
    bool isNextT() const;
    bool isAffects() const;
    bool isAffectsT() const;
};

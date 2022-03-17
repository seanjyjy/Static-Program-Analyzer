#pragma once

#include "WithVariable.h"
#include "SuperClause.h"

using namespace std;

class WithClause : public SuperClause {
private:
    WithVariable left, right;
public:
    WithClause(WithVariable, WithVariable);
    WithVariable getLeft();
    WithVariable getRight();

    // For SuperClause
    vector<QueryDeclaration> getSynonyms();
    bool hasSynonyms();
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

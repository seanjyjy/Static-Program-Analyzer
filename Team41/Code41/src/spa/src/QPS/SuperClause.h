#pragma once

#include "QueryDeclaration.h"
#include <vector>

class SuperClause {
public:
    virtual vector<QueryDeclaration> getSynonyms() = 0;
    virtual bool hasSynonyms() const = 0;
    virtual bool isWithClause() const = 0;
    virtual bool isSuchThatClause() const = 0;
    virtual bool isPatternClause() const = 0;
    virtual bool isFollows() const = 0;
    virtual bool isFollowsT() const = 0;
    virtual bool isParent() const = 0;
    virtual bool isParentT() const = 0;
    virtual bool isUsesS() const = 0;
    virtual bool isUsesP() const = 0;
    virtual bool isModifiesS() const = 0;
    virtual bool isModifiesP() const = 0;
    virtual bool isCalls() const = 0;
    virtual bool isCallsT() const = 0;
    virtual bool isNext() const = 0;
    virtual bool isNextT() const = 0;
    virtual bool isAffects() const = 0;
    virtual bool isAffectsT() const = 0;
};


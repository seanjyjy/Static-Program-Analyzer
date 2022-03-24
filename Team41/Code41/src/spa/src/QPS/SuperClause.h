#pragma once

#include <vector>
#include "QueryDeclaration.h"
#include "WithClause.h"
#include "QueryClause.h"
#include "PatternClause.h"

class SuperClause {
public:
    SuperClause(WithClause);
    SuperClause(PatternClause);
    SuperClause(QueryClause);
    const WithClause& getWithClause() const;
    const PatternClause& getPatternClause() const;
    const QueryClause& getSuchThatClause() const;
    int hash() const;
    bool equals(const SuperClause&) const;
    string toString() const;
    vector<QueryDeclaration> getSynonyms() const;
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
private:
    enum clause_type {
        SUCH_THAT, WITH, PATTERN
    };
    clause_type type;
    WithClause withClause;
    QueryClause suchThatClause;
    PatternClause patternClause;
};


#pragma once

#include <vector>
#include <unordered_set>
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
    bool equals(const SuperClause&) const;
    int hash() const;
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
    bool isUses() const;
    bool isModifiesS() const;
    bool isModifiesP() const;
    bool isModifies() const;
    bool isCalls() const;
    bool isCallsT() const;
    bool isNext() const;
    bool isNextT() const;
    bool isAffects() const;
    bool isAffectsT() const;
    bool operator==(const SuperClause &other) const {
        return this->equals(other);
    }
private:
    enum clause_type {
        SUCH_THAT, WITH, PATTERN
    };
    clause_type type;
    WithClause withClause;
    QueryClause suchThatClause;
    PatternClause patternClause;
};

namespace std {
    template<>
    struct hash<SuperClause> {
        std::size_t operator()(SuperClause const& g) const noexcept {
            return g.hash();
        }
    };
}
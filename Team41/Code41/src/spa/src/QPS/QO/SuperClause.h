#pragma once

#include <vector>
#include <unordered_set>
#include "QPS/QO/QueryDeclaration.h"
#include "WithClause.h"
#include "QPS/QO/QueryClause.h"
#include "QPS/QO/PatternClause.h"

class SuperClause {
public:
    SuperClause(WithClause);

    SuperClause(PatternClause);

    SuperClause(QueryClause);

    /**
     * Getter for the with clause.
     *
     * @return WithClause reference representing the with clause.
     */
    const WithClause &getWithClause() const;

    /**
     * Getter for the pattern clause.
     *
     * @return PatternClause reference representing the pattern clause.
     */
    const PatternClause &getPatternClause() const;

    /**
     * Getter for the such that clause.
     *
     * @return QueryClause reference representing the such that clause.
     */
    const QueryClause &getSuchThatClause() const;

    /**
     * Equivalence comparator for the SuperClause class.
     *
     * @return boolean indicating if equivalent.
     */
    bool equals(const SuperClause &) const;

    /**
     * Hash function for the super clause.
     *
     * @return int for the hash.
     */
    int hash() const;

    /**
     * Gets a string representation of the super clause.
     *
     * @return string representation.
     */
    string toString() const;

    /**
     * Getter for the clause's synonyms.
     *
     * @return vector of QueryDeclarations representing the clause's synonyms.
     */
    vector<QueryDeclaration> getSynonyms() const;

    /**
     * Methods indicating the clause has synonyms.
     *
     * @return boolean indicating the clause has synonyms.
     */
    bool hasSynonyms() const;

    /**
     * Methods indicating the clause is of the corresponding type.
     *
     * @return boolean indicating the clause is of the corresponding type.
     */
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

    void setSimplifiableClause(bool isSimplifiable);

private:
    enum clause_type {
        SUCH_THAT, WITH, PATTERN
    };
    clause_type type;
    // Composition pattern for clauses
    WithClause withClause;
    QueryClause suchThatClause;
    PatternClause patternClause;
};

namespace std {
    template<>
    struct hash<SuperClause> {
        std::size_t operator()(SuperClause const &g) const noexcept {
            return g.hash();
        }
    };
}

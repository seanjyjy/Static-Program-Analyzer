#pragma once

#include "ClauseGroups.h"
#include <functional>
#include <set>

/**
 * Custom comparator to sort clause groups.
 */
struct ClauseGroupComparator {
    bool operator()(ClauseGroup *lhs, ClauseGroup *rhs) const {
        // reducible groups go first
        if (lhs->canSimplify() && !rhs->canSimplify()) {
            return true;
        } else if (!lhs->canSimplify() && rhs->canSimplify()) {
            return false;
        } else if (lhs->canSimplify() && rhs->canSimplify() && lhs->size() != rhs->size()) {
            // if both reducible, smaller group goes first
            return lhs->size() < rhs->size();
        } else {
            // tiebreak by score
            return lhs->score() < rhs->score();
        }
    }
};

/**
 * Represents a group of clause groups, sorted by clause score.
 * Groups that can be evaluated fastest are placed first.
 */
class SortedGroups : public ClauseGroups {
private:
    mutable multiset<ClauseGroup *>::iterator it;
    multiset<ClauseGroup *, ClauseGroupComparator> sortedGroups;

    // maintains the invariant that the object always points to the next non-empty group, or groups.end() otherwise.
    void ensureInvariant();
public:
    /**
     * Creates a group of clause groups, sorted by clause score.
     * @param groups the clause groups, in any order (they will be sorted on initialization).
     */
    SortedGroups(vector<ClauseGroup*> groups);

    ~SortedGroups() override;

    SuperClause *pop() override;

    bool empty() override;

    ClauseGroup *front() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    string toString() const override;

    void print() const override;
};

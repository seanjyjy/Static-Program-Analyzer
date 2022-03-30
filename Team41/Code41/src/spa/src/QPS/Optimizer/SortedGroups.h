#pragma once

#include "ClauseGroups.h"
#include <functional>
#include <set>

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

class SortedGroups : public ClauseGroups {
private:
    mutable multiset<ClauseGroup *>::iterator it;
    multiset<ClauseGroup *, ClauseGroupComparator> sortedGroups;

    void ensureInvariant();
public:
    SortedGroups(vector<ClauseGroup*> groups);

    SuperClause *pop() override;

    bool empty() override;

    ClauseGroup *front() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    string toString() const override;

    void print() const override;
};

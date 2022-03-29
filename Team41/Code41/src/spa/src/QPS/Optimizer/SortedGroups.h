#pragma once

#include "ClauseGroups.h"
#include <functional>
#include <set>

struct AbstractGroupComparator {
    bool operator()(ClauseGroup *lhs, ClauseGroup *rhs) const {
        return lhs->score() < rhs->score();
    }
};

class SortedGroups : public ClauseGroups {
private:
    mutable multiset<ClauseGroup *>::iterator it;
    multiset<ClauseGroup *, AbstractGroupComparator> sortedGroups;
public:
    SortedGroups(vector<ClauseGroup*> groups);

    SuperClause *pop() override;

    bool empty() override;

    ClauseGroup *front() override;

    size_t currGroupSize() const override;

    bool isLastOfGroup() const override;

    string toString() const override;

    void print() const override;
};

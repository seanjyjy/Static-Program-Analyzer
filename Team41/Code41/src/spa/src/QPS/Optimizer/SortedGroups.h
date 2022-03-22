#pragma once

#include "AbstractGroups.h"
#include <functional>
#include <set>

struct AbstractGroupComparator {
    bool operator()(AbstractGroup *lhs, AbstractGroup *rhs) const {
        return lhs->score() < rhs->score();
    }
};

class SortedGroups : public AbstractGroups {
private:
    mutable multiset<AbstractGroup *>::iterator it;
    multiset<AbstractGroup *, AbstractGroupComparator> sortedGroups;
public:
    SortedGroups();

    void addGroup(AbstractGroup *group) override;

    SuperClause *pop() override;

    bool empty() override;

    AbstractGroup *front() override;

    string toString() const override;

    void print() const override;
};

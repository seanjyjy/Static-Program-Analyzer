#pragma once

#include "ClauseGroups.h"
#include "ClauseGroup.h"

class FifoGroups : public ClauseGroups {
private:
    vector<ClauseGroup *> groups;
    vector<ClauseGroup *>::iterator it;

    void ensureInvariant();
public:
    FifoGroups(vector<ClauseGroup*> groups);

    SuperClause *pop() override;

    bool empty() override;

    ClauseGroup *front() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    string toString() const override;

    void print() const override;
};

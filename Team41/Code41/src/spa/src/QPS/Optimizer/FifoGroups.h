#pragma once

#include "ClauseGroups.h"
#include "ClauseGroup.h"

/**
 * Represents a group of clause groups, each group consumed in fifo order.
 */
class FifoGroups : public ClauseGroups {
private:
    vector<ClauseGroup *> groups;
    vector<ClauseGroup *>::iterator it;

    // maintains the invariant that the object always points to the next non-empty group, or groups.end() otherwise.
    void ensureInvariant();

public:
    /**
     * Creates a group of clause groups to be consumed in FIFO order.
     * @param groups the clause groups in the same order they will be consumed.
     */
    FifoGroups(vector<ClauseGroup *> groups);

    ~FifoGroups() override;

    /**
     * Removes and returns the next clause in FIFO order from the current group.
     * @throws runtime_error if there are no more clauses to pop.
     */
    SuperClause *pop() override;

    bool empty() override;

    ClauseGroup *front() override;

    size_t currGroupSize() override;

    bool currGroupCanSimplify() override;

    bool isLastOfGroup() override;

    string toString() const override;

    void print() const override;
};

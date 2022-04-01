#pragma once

#include "ClauseGroup.h"
#include "QPS/SuperClause.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

/**
 * Represents a group of clauses consumed in fifo order.
 */
class FifoGroup : public ClauseGroup {
private:
    vector<SuperClause *> clauses;
    mutable int idx = 0;
    long long clauseScoreSum = 0;
public:
    /**
     * Creates an non-simplifiable FIFO group.
     * @param clauses the clauses to consume.
     */
    FifoGroup(vector<SuperClause *> clauses);

    /**
     * Creates a potentially simplifiable FIFO group.
     * @param clauses the clauses to consume.
     * @param canSimplify true if the group may be simplified, false otherwise.
     */
    FifoGroup(vector<SuperClause *> clauses, bool canSimplify);

    /**
     * Removes a clause in FIFO order from the group and returns it.
     * @throws runtime_error if the group is empty.
     */
    SuperClause *pop() const override;

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

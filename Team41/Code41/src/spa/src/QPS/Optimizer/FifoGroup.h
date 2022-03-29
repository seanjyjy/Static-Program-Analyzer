#pragma once

#include "ClauseGroup.h"
#include "QPS/SuperClause.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

class FifoGroup : public ClauseGroup {
private:
    vector<SuperClause *> clauses;
    mutable int idx = 0;
    long long clauseScoreSum = 0;
public:
    FifoGroup(vector<SuperClause *> clauses);
    FifoGroup(vector<SuperClause *> clauses, bool isEssential);

    SuperClause *pop() const override;

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

#pragma once

#include "AbstractGroup.h"
#include "QPS/SuperClause.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

class FifoGroup : public AbstractGroup {
private:
    vector<SuperClause *> clauses;
    mutable int idx = 0;
    long long clauseScoreSum = 0;
public:
    FifoGroup(vector<SuperClause *> clauses);

    SuperClause *pop() const override;

    bool empty() const override;

    size_t score() const override;

    void print() const override;

    string toString() const override;
};

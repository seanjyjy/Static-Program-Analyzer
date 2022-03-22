#pragma once

#include "AbstractGroup.h"
#include "ClauseScorer.h"
#include <set>
#include <numeric>

struct ClauseComparator {
    bool operator()(SuperClause *a, SuperClause *b) const {
        return ClauseScorer::score(a) < ClauseScorer::score(b);
    }
};

class SortedGroup : public AbstractGroup {
private:
    multiset<SuperClause *, ClauseComparator> clauses;
    mutable multiset<SuperClause *>::iterator it;
    long long clauseScoreSum = 0;
public:
    SortedGroup(vector<SuperClause *> initClauses);

    SuperClause *pop() const override; // for non-const contexts
    bool empty() const override;

    size_t score() const override;

    void print() const override;

    string toString() const override;
};

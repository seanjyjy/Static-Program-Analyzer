#pragma once

#include "ClauseGroup.h"
#include "ClauseScorer.h"
#include <set>
#include <numeric>

struct ClauseComparator {
    bool operator()(SuperClause *a, SuperClause *b) const {
        return ClauseScorer::score(a) < ClauseScorer::score(b);
    }
};

class SortedGroup : public ClauseGroup {
private:
    multiset<SuperClause *, ClauseComparator> clauses;
    mutable multiset<SuperClause *>::iterator it;
    long long clauseScoreSum = 0;
public:
    SortedGroup(vector<SuperClause *> initClauses);
    SortedGroup(vector<SuperClause *> initClauses, bool canSimplify);

    SuperClause *pop() const override; // for non-const contexts

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

#pragma once

#include "PKBAdapter.h"
#include "ClauseGroup.h"
#include "TableEstimate.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

class PKBAwareGroup : public ClauseGroup {
private:
    vector<SuperClause *> clauses;
    mutable vector<bool> isClauseUsed;
    mutable TableEstimate table;
    long long clauseScoreSum = 0;
    mutable long long clausesPopped = 0;
    bool isEssential = false;
public:
    PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter);
    PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter, bool isEssential);

    SuperClause *pop() const override;

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

#pragma once

#include "PKBAdapter.h"
#include "AbstractGroup.h"
#include "TableEstimate.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

class PKBAwareGroup : public AbstractGroup {
private:
    vector<SuperClause *> clauses;
    mutable vector<bool> isClauseUsed;
    mutable TableEstimate table;
    long long clauseScoreSum = 0;
    mutable long long clausesPopped = 0;
public:
    PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter);

    SuperClause *pop() const override;

    bool empty() const override;

    size_t score() const override;

    void print() const override;

    string toString() const override;
};

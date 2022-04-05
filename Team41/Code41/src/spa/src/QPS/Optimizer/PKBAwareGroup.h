#pragma once

#include "PKBAdapter.h"
#include "ClauseGroup.h"
#include "TableEstimate.h"
#include "ClauseScorer.h"
#include <utility>
#include <numeric>

/**
 * Represents a clause group that has access to information from the PKB.
 * This information is mainly used to retrieve clauses that result in smallest intermediate tables.
 */
class PKBAwareGroup : public ClauseGroup {
private:
    vector<SuperClause *> clauses;
    mutable vector<bool> isClauseUsed; // checks if a clause is consumed
    mutable TableEstimate table; // estimate of the table built as clauses are consumed
    long long clauseScoreSum = 0; // total score of the clause group; used for sorting
    mutable long long clausesPopped = 0; // used to check if all clauses have been consumed
public:
    /**
     * Creates a non-simplifiable group of clauses to consume that may access the PKB.
     * @param initClauses the clauses to consume.
     * @param pkbAdapter the interface to the PKB.
     */
    PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter);

    /**
     * Creates a potentially simplifiable group of clauses to consume, that may access the PKB.
     * @param initClauses the clauses to consume.
     * @param pkbAdapter the interface to the PKB.
     * @param canSimplify whether the group may be simplified by the query evaluator.
     */
    PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter, bool canSimplify);

    /**
     * Greedily removes and returns the next clause that produces the smallest intermediate table.
     */
    SuperClause *pop() const override;

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

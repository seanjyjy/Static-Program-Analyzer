#pragma once

#include "ClauseGroup.h"
#include "ClauseScorer.h"
#include <set>
#include <numeric>

/**
 * Custom comparator to sort clauses based on their clause score.
 */
struct ClauseComparator {
    bool operator()(SuperClause *a, SuperClause *b) const {
        return ClauseScorer::score(a) < ClauseScorer::score(b);
    }
};

/**
 * Represents a clause group with clauses sorted by their clause score.
 * See ClauseScorer.h for details on clause scoring.
 */
class SortedGroup : public ClauseGroup {
private:
    multiset<SuperClause *, ClauseComparator> clauses;
    mutable multiset<SuperClause *>::iterator it;
    long long clauseScoreSum = 0; // for sorting
public:
    /**
     * Creates a non-simplifiable group of clauses, sorted by their clause score.
     * @param initClauses the clauses, in any order (they will be sorted on initialization).
     */
    SortedGroup(vector<SuperClause *> initClauses);

    /**
     * Creates a potentially simplifiable group of clauses, sorted by their clause score.
     * @param initClauses the clauses, in any order (they will be sorted on initialization).
     * @param canSimplify true if the clause group may be simplified by the query evaluator, false otherwise.
     */
    SortedGroup(vector<SuperClause *> initClauses, bool canSimplify);

    /**
     * Removes and returns the next clause that has the greatest score.
     * A greater score means the clause should be able to be evaluated in the least amount of time.
     */
    SuperClause *pop() const override;

    size_t size() const override;

    bool empty() const override;

    size_t score() const override;

    bool isLast() const override;

    void print() const override;

    string toString() const override;
};

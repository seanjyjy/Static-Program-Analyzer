#pragma once

#include <iostream>
#include "QPS/SuperClause.h"

/**
 * Represents an abstraction of a group of clauses that can be consumed.
 * Grants certain properties on clauses, such as being sorted.
 */
class ClauseGroup {
private:
    bool isSimplifiable = false; // for optimization purposes
public:
    ClauseGroup();

    /**
     * Creates an empty clause group.
     * @param canSimplify whether the clause group is simplifiable - if true, the query evaluator does not need to
     * naively evaluate the clauses in this group during table evaluation.
     */
    ClauseGroup(bool canSimplify);

    /**
     * Removes a clause from the group and returns it.
     */
    virtual SuperClause *pop() const = 0;

    /**
     * Returns the total number of clauses in the group. Unaffected by popping clauses from the group.
     */
    virtual size_t size() const = 0;

    /**
     * Returns true if there are no more clauses to pop, false otherwise.
     */
    virtual bool empty() const = 0;

    /**
     * Returns a numeric score estimating the number of rows from evaluating all clauses into a table.
     * Higher scores mean the group should be evaluated first, and produces less rows.
     */
    virtual size_t score() const = 0;

    /**
     * Returns true if there is only one unpopped clause left in the group, false otherwise.
     */
    virtual bool isLast() const = 0;

    /**
     * Returns whether the clause group may be simplified - whether the query evaluator can choose not to
     * naively evaluate the clauses into a table.
     */
    [[nodiscard]] bool canSimplify() const;

    /**
     * Prints a description of all clauses in the group. Mainly for debugging.
     */
    virtual void print() const = 0;

    /**
     * Returns a string description of all clauses in the group. Mainly for debugging.
     */
    virtual string toString() const = 0;
};



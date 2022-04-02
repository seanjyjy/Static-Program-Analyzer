#pragma once

#include "ClauseGroup.h"

/**
 * Represents an abstraction of a group of clause groups that can be consumed, one group at a time.
 * Grants certain properties on clause groups, such as being sorted.
 */
class ClauseGroups {
public:
    /**
     * Returns the next clause to be consumed, but does not remove it from the groups abstraction.
     */
    virtual ClauseGroup *front() = 0;

    /**
     * Removes and returns the next clause to be consumed.
     */
    virtual SuperClause *pop() = 0;

    /**
     * Returns true if there are no more clauses to consume in all groups, or false otherwise.
     */
    virtual bool empty() = 0;

    /**
     * Returns the total size of the current group being consumed.
     * Unaffected by clause popping, unless the clause pop causes us to start consuming the next group
     * - then this method returns the size of the next group.
     */
    virtual size_t currGroupSize() = 0;

    /**
     * Returns true if the current group can be simplified by the query evaluator, false otherwise.
     */
    virtual bool currGroupCanSimplify() = 0;

    /**
     * Returns true if the next clause returned by popping is the last unconsumed clause of its group, false otherwise.
     */
    virtual bool isLastOfGroup() = 0;

    /**
     * Returns a string representation of all clause groups. Mainly for debugging.
     */
    virtual string toString() const = 0;

    /**
     * Prints a representation of all clause groups. Mainly for debugging.
     */
    virtual void print() const = 0;
};

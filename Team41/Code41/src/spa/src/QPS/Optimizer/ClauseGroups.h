#pragma once

#include "ClauseGroup.h"

class ClauseGroups {
public:
    virtual ClauseGroup *front() = 0;

    virtual SuperClause *pop() = 0;

    virtual bool empty() = 0;

    virtual size_t currGroupSize() = 0;

    virtual bool isLastOfGroup() = 0;

    virtual string toString() const = 0;

    virtual void print() const = 0;
};

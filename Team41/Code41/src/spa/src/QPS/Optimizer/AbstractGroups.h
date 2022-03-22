#pragma once

#include "AbstractGroup.h"

class AbstractGroups {
public:
    virtual AbstractGroup *front() = 0;

    virtual SuperClause *pop() = 0;

    virtual bool empty() = 0;

    virtual string toString() const = 0;

    virtual void print() const = 0;
};

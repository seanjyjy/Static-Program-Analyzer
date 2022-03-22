#pragma once

#include <iostream>
#include "QPS/SuperClause.h"

class AbstractGroup {
public:
    virtual SuperClause *pop() const = 0;

    virtual bool empty() const = 0;

    virtual size_t score() const = 0;

    virtual void print() const = 0;

    virtual string toString() const = 0;
};



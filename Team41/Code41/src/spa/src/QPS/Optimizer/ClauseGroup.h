#pragma once

#include <iostream>
#include "QPS/SuperClause.h"

class ClauseGroup {
private:
    bool isSimplifiable = false; // for optimization purposes
public:
    ClauseGroup();

    ClauseGroup(bool isEssential);

    virtual SuperClause *pop() const = 0;

    virtual size_t size() const = 0;

    virtual bool empty() const = 0;

    virtual size_t score() const = 0;

    virtual bool isLast() const = 0;

    [[nodiscard]] bool canSimplify() const;

    virtual void print() const = 0;

    virtual string toString() const = 0;
};



#pragma once

#include "ClauseGroups.h"

class OptimizedQueryObject {
    ClauseGroups clauseGroup;
public:
    OptimizedQueryObject();
    OptimizedQueryObject(ClauseGroups &cg);
};

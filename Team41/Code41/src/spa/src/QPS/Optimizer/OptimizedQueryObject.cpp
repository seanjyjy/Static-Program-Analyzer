#include "OptimizedQueryObject.h"

OptimizedQueryObject::OptimizedQueryObject() = default;

OptimizedQueryObject::OptimizedQueryObject(ClauseGroups &cg): clauseGroup(cg) {}

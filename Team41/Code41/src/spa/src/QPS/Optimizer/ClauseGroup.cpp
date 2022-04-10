#include "ClauseGroup.h"

ClauseGroup::ClauseGroup() = default;

ClauseGroup::ClauseGroup(bool canSimplify) : isSimplifiable(canSimplify) {
}

bool ClauseGroup::canSimplify() const {
    return isSimplifiable;
}

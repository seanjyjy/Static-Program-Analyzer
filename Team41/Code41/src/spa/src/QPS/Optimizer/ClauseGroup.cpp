#include "ClauseGroup.h"

ClauseGroup::ClauseGroup() = default;

ClauseGroup::ClauseGroup(bool isEssential): isEssential(isEssential) {
}

bool ClauseGroup::canSimplify() const {
    return !isEssential;
}

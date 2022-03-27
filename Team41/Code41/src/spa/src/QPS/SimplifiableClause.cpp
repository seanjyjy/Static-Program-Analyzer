#include "SimplifiableClause.h"

bool SimplifiableClause::canSimplifyClause() const {
    return canSimplify;
}

void SimplifiableClause::setSimplified(bool canSimplifyClause) {
    this->canSimplify = canSimplifyClause;
}
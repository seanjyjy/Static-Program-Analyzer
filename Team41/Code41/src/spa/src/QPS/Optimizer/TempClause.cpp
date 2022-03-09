#include "TempClause.h"

TempClause::TempClause(QueryClause *queryClause): qc(queryClause) {}

TempClause::TempClause(PatternClause *patternClause): pc(patternClause) {}

vector<string> TempClause::getSynonyms() {
    return vector<string>();
}

bool TempClause::hasSynonyms() const {
    return false;
}

bool TempClause::isWithClause() const {
    return false;
}

bool TempClause::isSuchThatClause() const {
    return false;
}

bool TempClause::isPatternClause() const {
    return false;
}

bool TempClause::isFollows() const {
    return false;
}

bool TempClause::isFollowsT() const {
    return false;
}

bool TempClause::isParent() const {
    return false;
}

bool TempClause::isParentT() const {
    return false;
}

bool TempClause::isUsesS() const {
    return false;
}

bool TempClause::isUsesP() const {
    return false;
}

bool TempClause::isModifiesS() const {
    return false;
}

bool TempClause::isModifiesP() const {
    return false;
}

bool TempClause::isCalls() const {
    return false;
}

bool TempClause::isCallsT() const {
    return false;
}

bool TempClause::isNext() const {
    return false;
}

bool TempClause::isNextT() const {
    return false;
}

bool TempClause::isAffects() const {
    return false;
}

bool TempClause::isAffectsT() const {
    return false;
}

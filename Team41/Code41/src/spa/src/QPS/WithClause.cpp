#include "WithClause.h"

WithClause::WithClause(WithVariable left, WithVariable right):SuperClause(), left(left), right(right){}

WithVariable WithClause::getLeft() {
    return left;
}

WithVariable WithClause::getRight() {
    return right;
}

//// For SuperClause

vector<QueryDeclaration> WithClause::getSynonyms() {
    vector<QueryDeclaration> out;
    if (left.getType() == WithVariable::ATTR_REF)
        out.push_back(left.getSynonym());
    if (right.getType() == WithVariable::ATTR_REF)
        out.push_back(right.getSynonym());
    return out;
}

bool WithClause::hasSynonyms() {
    return left.getType() == WithVariable::ATTR_REF || right.getType() == WithVariable::ATTR_REF;
}

bool WithClause::isWithClause() const {
    return true;
}

bool WithClause::isSuchThatClause() const {
    return false;
}

bool WithClause::isPatternClause() const {
    return false;
}

bool WithClause::isFollows() const {
    return false;
}

bool WithClause::isFollowsT() const {
    return false;
}

bool WithClause::isParent() const {
    return false;
}
bool WithClause::isParentT() const {
    return false;
}

bool WithClause::isUsesS() const {
    return false;
}

bool WithClause::isUsesP() const {
    return false;
}

bool WithClause::isModifiesS() const {
    return false;
}

bool WithClause::isModifiesP() const {
    return false;
}

bool WithClause::isCalls() const {
    return false;
}

bool WithClause::isCallsT() const {
    return false;
}

bool WithClause::isNext() const {
    return false;
}

bool WithClause::isNextT() const {
    return false;
}

bool WithClause::isAffects() const {
    return false;
}

bool WithClause::isAffectsT() const {
    return false;
}

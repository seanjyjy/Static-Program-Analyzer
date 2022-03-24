#include "WithClause.h"

WithClause::WithClause() {}

WithClause::WithClause(WithVariable left, WithVariable right): left(std::move(left)), right(std::move(right)){}

WithVariable WithClause::getLeft() const {
    return left;
}

WithVariable WithClause::getRight() const {
    return right;
}

vector<QueryDeclaration> WithClause::getSynonyms() const {
    vector<QueryDeclaration> out;
    if (left.getType() == WithVariable::ATTR_REF)
        out.push_back(left.getSynonym());
    if (right.getType() == WithVariable::ATTR_REF)
        out.push_back(right.getSynonym());
    return out;
}

bool WithClause::hasSynonyms() const {
    return left.getType() == WithVariable::ATTR_REF || right.getType() == WithVariable::ATTR_REF;
}

int WithClause::hash() const {
    int out = (int)std::hash<string>{}("with")
        ^ (int)std::hash<int>{} (left.getType())
        ^ (int)std::hash<int>{} (right.getType());

    if (left.isIdentifier())
        out ^= (int)std::hash<string>{} (left.getIdent());
    if (right.isIdentifier())
        out ^= (int)std::hash<string>{} (right.getIdent());

    if (left.isInteger())
        out ^= (int)std::hash<int>{} (left.getInteger());
    if (right.isInteger())
        out ^= (int)std::hash<int>{} (right.getInteger());

    if (left.isAttrRef())
        out ^= (int)std::hash<int>{} (left.getAttr());
    if (right.isAttrRef())
        out ^= (int)std::hash<int>{} (right.getAttr());

    return out;
}

bool WithClause::equals(WithClause other) const {
    return left.equals(other.getLeft())
        && right.equals(other.getRight());
}

string WithClause::toString() const {
    return "with " + left.toString() + " " + right.toString();
}
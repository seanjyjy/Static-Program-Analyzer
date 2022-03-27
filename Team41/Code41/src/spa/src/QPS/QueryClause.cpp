#include "QueryClause.h"

#include <utility>

QueryClause::QueryClause(clause_type type, ClauseVariable left, ClauseVariable right) :
    left(std::move(left)), right(std::move(right)), type(type) {}

QueryClause::QueryClause() {}

ClauseVariable QueryClause::getLeftClauseVariable() const {
    return left;
}

ClauseVariable QueryClause::getRightClauseVariable() const {
    return right;
}

QueryClause::clause_type QueryClause::getType() const {
    return type;
}

string QueryClause::typeToString() const {
    if (type == clause_type::follows) return "Follows";
    if (type == clause_type::followsT) return "Follows*";
    if (type == clause_type::parent) return "Parent";
    if (type == clause_type::parentT) return "Parent*";
    if (type == clause_type::usesS) return "UsesS";
    if (type == clause_type::usesP) return "UsesP";
    if (type == clause_type::modifiesS) return "ModifiesS";
    if (type == clause_type::modifiesP) return "ModifiesP";
    if (type == clause_type::calls) return "Calls";
    if (type == clause_type::callsT) return "Calls*";
    if (type == clause_type::next) return "Next";
    if (type == clause_type::nextT) return "Next*";
    if (type == clause_type::affects) return "Affects";
    if (type == clause_type::affectsT) return "Affects*";
    throw runtime_error("no valid string mapping for given clause type");
}

string QueryClause::toString() const {
    string lhs = getLeftClauseVariable().getLabel();
    string rhs = getRightClauseVariable().getLabel();
    return typeToString() + "(" + lhs + "," + rhs + ")";
}

void QueryClause::print() {
    cout << toString() << endl;
}

//// For SuperClause

vector<QueryDeclaration> QueryClause::getSynonyms() const {
    vector<QueryDeclaration> out;
    if (left.isSynonym())
        out.push_back(left.getQueryDeclaration());
    if (right.isSynonym())
        out.push_back(right.getQueryDeclaration());
    return out;
}

bool QueryClause::hasSynonyms() const {
    return left.isSynonym() || right.isSynonym();
}

int QueryClause::hash() const {
    int out = (int)std::hash<string>{}("suchthat");
    out ^= (int)std::hash<int>{}(type);
    out ^= (int)std::hash<int>{}(left.getType());
    out ^= (int)std::hash<int>{}(right.getType());
    return out;
}

bool QueryClause::equals(QueryClause other) const {
    if (type != other.getType())
        return false;
    return left.equals(other.getLeftClauseVariable())
        && right.equals(other.getRightClauseVariable());
}

bool QueryClause::isBooleanClause() const {
    switch (type) {
        case usesP:
        case modifiesP:
            return left.isIdentifier() && (right.isIdentifier() || right.isWildCard());
        case usesS:
        case modifiesS:
            return left.isInteger() && (right.isIdentifier() || right.isWildCard());
        case calls:
        case callsT:
            return (left.isIdentifier() && (right.isIdentifier() || right.isWildCard())) ||
                (left.isWildCard() && (right.isIdentifier() || right.isWildCard()));
        default:
            return (left.isInteger() && (right.isInteger() || right.isWildCard())) ||
                (left.isWildCard() && (right.isInteger() || right.isWildCard()));
    }
}

QueryClause QueryClause::generateSimplifiedSelf() const {
    ClauseVariable leftClause = left.isSynonym() ? left.convertWildCard() : left;
    ClauseVariable rightClause = right.isSynonym() ? right.convertWildCard() : right;
    return {type, leftClause, rightClause};
}

// TODO change when kendrick is okay!
bool QueryClause::canSimplifyClause() const {
    return false;
}

void QueryClause::setSimplified(bool canSimplifyClause) {
    this->canSimplify = canSimplifyClause;
}


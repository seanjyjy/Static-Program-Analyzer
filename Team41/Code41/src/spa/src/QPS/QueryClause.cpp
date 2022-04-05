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

string QueryClause::toString() const {
    string lhs = getLeftClauseVariable().getLabel();
    string rhs = getRightClauseVariable().getLabel();
    return "QueryClause(" + lhs + "," + rhs + ")";
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
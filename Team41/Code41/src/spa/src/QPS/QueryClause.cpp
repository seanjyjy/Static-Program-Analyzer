#include "QueryClause.h"

#include <utility>

QueryClause::QueryClause(clause_type type, ClauseVariable left, ClauseVariable right) :
    type(type), left(std::move(left)), right(std::move(right)) {}

ClauseVariable QueryClause::getLeftClauseVariable() const {
    return left;
}

ClauseVariable QueryClause::getRightClauseVariable() const {
    return right;
}

string QueryClause::typeToString(QueryClause::clause_type &c) {
    if (c == clause_type::follows) return "Follows";
    if (c == clause_type::followsT) return "Follows*";
    if (c == clause_type::parent) return "Parent";
    if (c == clause_type::parentT) return "Parent*";
    if (c == clause_type::usesS) return "UsesS";
    if (c == clause_type::usesP) return "UsesP";
    if (c == clause_type::modifiesS) return "ModifiesS";
    if (c == clause_type::modifiesP) return "ModifiesP";
    if (c == clause_type::calls) return "Calls";
    if (c == clause_type::callsT) return "Calls*";
    if (c == clause_type::next) return "Next";
    if (c == clause_type::nextT) return "Next*";
    if (c == clause_type::affects) return "Affects";
    if (c == clause_type::affectsT) return "Affects*";
    throw runtime_error("no valid string mapping for given clause type");
}

string QueryClause::toString() {
    string lhs = getLeftClauseVariable().getLabel();
    string rhs = getRightClauseVariable().getLabel();
    return typeToString(type) + "(" + lhs + "," + rhs + ")";
}

void QueryClause::print() {
    cout << toString() << endl;
}

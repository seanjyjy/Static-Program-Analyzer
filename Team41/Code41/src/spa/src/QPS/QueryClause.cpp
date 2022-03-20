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

QueryClause::clause_type QueryClause::getType() {
    return type;
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

//// For SuperClause

vector<QueryDeclaration> QueryClause::getSynonyms() {
    vector<QueryDeclaration> out;
    if (left.isSynonym())
        out.push_back(left.getQueryDeclaration());
    if (right.isSynonym())
        out.push_back(right.getQueryDeclaration());
    return out;
}

bool QueryClause::hasSynonyms() {
    return left.isSynonym() || right.isSynonym();
}

bool QueryClause::isWithClause() const {
    return false;
};

bool QueryClause::isSuchThatClause() const {
    return true;
}

bool QueryClause::isPatternClause() const {
    return false;
}

bool QueryClause::isFollows() const {
    return type == follows;
}

bool QueryClause::isFollowsT() const {
    return type == followsT;
}

bool QueryClause::isParent() const {
    return type == parent;
}
bool QueryClause::isParentT() const {
    return type == parentT;
}

bool QueryClause::isUsesS() const {
    return type == usesS;
}

bool QueryClause::isUsesP() const {
    return type == usesP;
}

bool QueryClause::isModifiesS() const {
    return type == modifiesS;
}

bool QueryClause::isModifiesP() const {
    return type == modifiesP;
}

bool QueryClause::isCalls() const {
    return type == calls;
}

bool QueryClause::isCallsT() const {
    return type == callsT;
}

bool QueryClause::isNext() const {
    return type == next;
}

bool QueryClause::isNextT() const {
    return type == nextT;
}

bool QueryClause::isAffects() const {
    return type == affects;
}

bool QueryClause::isAffectsT() const {
    return type == affectsT;
}

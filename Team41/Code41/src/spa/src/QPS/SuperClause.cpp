#include "SuperClause.h"

SuperClause::SuperClause(WithClause wc): withClause(wc) {
    type = WITH;
}

SuperClause::SuperClause(PatternClause pc): patternClause(pc) {
    type = PATTERN;
}

SuperClause::SuperClause(QueryClause qc): suchThatClause(qc) {
    type = SUCH_THAT;
}

const WithClause& SuperClause::getWithClause() const {
    return withClause;
}

const PatternClause& SuperClause::getPatternClause() const {
    return patternClause;
}

const QueryClause& SuperClause::getSuchThatClause() const {
    return suchThatClause;
}

int SuperClause::hash() const {
    // todo: make use of underlying clause data
    return 0;
}

bool SuperClause::equals(SuperClause other) const {
    // todo: compare underlying clause type and data
    return false;
}

string SuperClause::toString() const {
    // todo: print data of underlying clause
    return "help me\n";
}

vector<QueryDeclaration> SuperClause::getSynonyms() const {
    // todo
    return vector<QueryDeclaration>();
}

bool SuperClause::hasSynonyms() const {
    return false;
}

bool SuperClause::isWithClause() const {
    return type == WITH;
}

bool SuperClause::isSuchThatClause() const {
    return type == SUCH_THAT;
}

bool SuperClause::isPatternClause() const {
    return type == PATTERN;
}

bool SuperClause::isFollows() const {
    return type == SUCH_THAT
        && suchThatClause.getType() == QueryClause::follows;
}

bool SuperClause::isFollowsT() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::followsT;
}

bool SuperClause::isParent() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::parent;
}

bool SuperClause::isParentT() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::parentT;
}

bool SuperClause::isUsesS() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::usesS;
}

bool SuperClause::isUsesP() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::usesP;
}

bool SuperClause::isModifiesS() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::modifiesS;
}

bool SuperClause::isModifiesP() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::modifiesP;
}

bool SuperClause::isCalls() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::calls;
}

bool SuperClause::isCallsT() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::callsT;
}

bool SuperClause::isNext() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::next;
}

bool SuperClause::isNextT() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::nextT;
}

bool SuperClause::isAffects() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::affects;
}

bool SuperClause::isAffectsT() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::affectsT;
}

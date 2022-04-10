#include "SuperClause.h"

#include <utility>

SuperClause::SuperClause(WithClause wc) : withClause(std::move(wc)) {
    type = WITH;
}

SuperClause::SuperClause(PatternClause pc) : patternClause(std::move(pc)) {
    type = PATTERN;
}

SuperClause::SuperClause(QueryClause qc) : suchThatClause(std::move(qc)) {
    type = SUCH_THAT;
}

const WithClause &SuperClause::getWithClause() const {
    return withClause;
}

const PatternClause &SuperClause::getPatternClause() const {
    return patternClause;
}

const QueryClause &SuperClause::getSuchThatClause() const {
    return suchThatClause;
}

int SuperClause::hash() const {
    if (isWithClause())
        return withClause.hash();
    if (isPatternClause())
        return patternClause.hash();
    if (isSuchThatClause())
        return suchThatClause.hash();
    return 0;
}

bool SuperClause::equals(const SuperClause &other) const {
    if (isWithClause() && other.isWithClause())
        return withClause.equals(other.getWithClause());
    if (isPatternClause() && other.isPatternClause())
        return patternClause.equals(other.getPatternClause());
    if (isSuchThatClause() && other.isSuchThatClause())
        return suchThatClause.equals(other.getSuchThatClause());
    return false;
}

string SuperClause::toString() const {
    if (isWithClause())
        return withClause.toString();
    if (isPatternClause())
        return patternClause.toString();
    if (isSuchThatClause())
        return suchThatClause.toString();
    return "imma string\n";
}

vector<QueryDeclaration> SuperClause::getSynonyms() const {
    if (isWithClause())
        return withClause.getSynonyms();
    if (isPatternClause())
        return patternClause.getSynonyms();
    if (isSuchThatClause())
        return suchThatClause.getSynonyms();
    return {};
}

bool SuperClause::hasSynonyms() const {
    if (isWithClause())
        return withClause.hasSynonyms();
    if (isPatternClause())
        return patternClause.hasSynonyms();
    if (isSuchThatClause())
        return suchThatClause.hasSynonyms();

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

bool SuperClause::isUses() const {
    return type == SUCH_THAT
           && (suchThatClause.getType() == QueryClause::usesP ||
               suchThatClause.getType() == QueryClause::usesS ||
               suchThatClause.getType() == QueryClause::generic_uses);
}

bool SuperClause::isModifiesS() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::modifiesS;
}

bool SuperClause::isModifiesP() const {
    return type == SUCH_THAT
           && suchThatClause.getType() == QueryClause::modifiesP;
}

bool SuperClause::isModifies() const {
    return type == SUCH_THAT
           && (suchThatClause.getType() == QueryClause::modifiesP ||
               suchThatClause.getType() == QueryClause::modifiesS ||
               suchThatClause.getType() == QueryClause::generic_modifies);
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

void SuperClause::setSimplifiableClause(bool isSimplifiable) {
    if (isWithClause())
        withClause.setSimplified(isSimplifiable);
    else if (isPatternClause())
        patternClause.setSimplified(isSimplifiable);
    else if (isSuchThatClause())
        suchThatClause.setSimplified(isSimplifiable);
}

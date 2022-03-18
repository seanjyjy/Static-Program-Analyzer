#include "PatternClause.h"

PatternClause::PatternClause(QueryDeclaration synonym, ClauseVariable lhs, vector<PatternVariable> rhs) :
        synonym(synonym), lhs(lhs), rhs(rhs) {}

PatternClause::PatternClause(QueryDeclaration synonym, ClauseVariable lhs, QueryDeclaration lhsQD, vector<PatternVariable> rhs) :
    synonym(synonym), lhs(lhs), lhsQD(lhsQD), rhs(rhs) {}

QueryDeclaration PatternClause::getSynonym() const { return synonym; }
ClauseVariable PatternClause::getLHS() const { return lhs; }
vector<PatternVariable> PatternClause::getRHS() const { return rhs; }

//// For SuperClause

vector<QueryDeclaration> PatternClause::getSynonyms() {
    vector<QueryDeclaration> out;
    out.push_back(synonym);
    if (lhs.isSynonym())
        out.push_back(lhsQD);
    return out;
}

bool PatternClause::hasSynonyms() {
    return true; // A pattern clause should always have synonyms
}

bool PatternClause::isWithClause() const {
    return false;
};

bool PatternClause::isSuchThatClause() const {
    return false;
}

bool PatternClause::isPatternClause() const {
    return true;
}

bool PatternClause::isFollows() const {
    return false;
}

bool PatternClause::isFollowsT() const {
    return false;
}

bool PatternClause::isParent() const {
    return false;
}
bool PatternClause::isParentT() const {
    return false;
}

bool PatternClause::isUsesS() const {
    return false;
}

bool PatternClause::isUsesP() const {
    return false;
}

bool PatternClause::isModifiesS() const {
    return false;
}

bool PatternClause::isModifiesP() const {
    return false;
}

bool PatternClause::isCalls() const {
    return false;
}

bool PatternClause::isCallsT() const {
    return false;
}

bool PatternClause::isNext() const {
    return false;
}

bool PatternClause::isNextT() const {
    return false;
}

bool PatternClause::isAffects() const {
    return false;
}

bool PatternClause::isAffectsT() const {
    return false;
}

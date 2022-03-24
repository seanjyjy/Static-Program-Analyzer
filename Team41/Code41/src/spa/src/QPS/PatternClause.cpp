#include "PatternClause.h"

PatternClause::PatternClause(QueryDeclaration synonym, ClauseVariable lhs, vector<PatternVariable> rhs) :
        synonym(synonym), lhs(lhs), rhs(rhs) {}

PatternClause::PatternClause() {}

QueryDeclaration PatternClause::getSynonym() const { return synonym; }
ClauseVariable PatternClause::getLHS() const { return lhs; }
vector<PatternVariable> PatternClause::getRHS() const { return rhs; }

//// For SuperClause

vector<QueryDeclaration> PatternClause::getSynonyms() const {
    vector<QueryDeclaration> out;
    out.push_back(synonym);
    if (lhs.isSynonym())
        out.push_back(lhs.getQueryDeclaration());
    return out;
}

bool PatternClause::hasSynonyms() const {
    return true; // A pattern clause should always have synonyms
}

bool PatternClause::equals(PatternClause other) const {
    if (!synonym.equals(other.getSynonym()))
        return false;
    // Compare left variable
    if (!lhs.equals(other.getLHS()))
        return false;
    // Compare right variable(s)
    vector<PatternVariable> otherRight = other.getRHS();
    for (int i = 0; i < (int)rhs.size(); i++) {
        if (!rhs.at(i).equals(otherRight.at(i)))
            return false;
    }
    return true;
}

string PatternClause::toString() const {
    string out = "pattern";
    out += " " + synonym.getSynonym();
    if (lhs.isIdentifier())
        out += "L:" + lhs.getLabel();
    if (lhs.isWildCard())
        out += "L:wildcard";
    return out;
}

int PatternClause::hash() const {
    int out = (int)std::hash<string>{}("pattern");
    out ^= (int)std::hash<int>{}(synonym.getType());
    out ^= (int)std::hash<int>{}(lhs.getType());
    for (int i = 0; i < (int)rhs.size(); i++) {
        out ^= (int)std::hash<int>{}(rhs.at(i).getType());
    }
    return out;
}

#include "ClauseScorer.h"

#include <stdexcept>

using namespace std;

long ClauseScorer::score(const TempClause &tc) {
    if (!tc.hasSynonyms()) {
        return SCORE_CLAUSE_NO_SYNONYMS;
    } else if (tc.isWithClause()) {
        return scoreWithCl(tc);
    } else if (tc.isSuchThatClause()) {
        return scoreSuchThatCl(tc);
    } else if (tc.isPatternClause()) {
        return scorePatternCl(tc);
    }
    return 0;
}

// all with-clauses are treated the same, i.e assigned the same score
long ClauseScorer::scoreWithCl(const TempClause &tc) {
    return SCORE_WITH_CLAUSE;
}

long ClauseScorer::scoreSuchThatCl(const TempClause &tc) {
    SuchThatType type = stClauseToType(tc);
    vector<ClauseScorer::SuchThatType> ranks = getStRanks();
    size_t idx = find(ranks.begin(), ranks.end(), type) - ranks.begin();
    long piece = SCORE_SUCHTHAT_CLAUSE / (long) ranks.size();
    return SCORE_SUCHTHAT_CLAUSE - (idx * piece); // lower index = higher score
}

// all pattern-clauses are treated the same, i.e assigned the same score
long ClauseScorer::scorePatternCl(const TempClause &tc) {
    return SCORE_PATTERN_CLAUSE;
}

ClauseScorer::SuchThatType ClauseScorer::stClauseToType(const TempClause &tc) {
    if (!tc.isSuchThatClause()) throw runtime_error("cannot convert non-suchthat clause to its type");
    if (tc.isAffects()) return SuchThatType::Affects;
    if (tc.isAffectsT()) return SuchThatType::AffectsT;
    if (tc.isCalls()) return SuchThatType::Calls;
    if (tc.isCallsT()) return SuchThatType::CallsT;
    if (tc.isFollows()) return SuchThatType::Follows;
    if (tc.isFollowsT()) return SuchThatType::FollowsT;
    if (tc.isModifiesP() || tc.isModifiesS()) return SuchThatType::Modifies;
    if (tc.isNext()) return SuchThatType::Next;
    if (tc.isNextT()) return SuchThatType::NextT;
    if (tc.isParent()) return SuchThatType::Parent;
    if (tc.isParentT()) return SuchThatType::ParentT;
    if (tc.isUsesP() || tc.isUsesS()) return SuchThatType::Uses;
    throw runtime_error("could not find matching for suchthat clause");
}

vector<ClauseScorer::SuchThatType> ClauseScorer::getStRanks() {
    const static vector<ClauseScorer::SuchThatType> stRanks = {
            SuchThatType::Follows,
            SuchThatType::Parent,
            SuchThatType::Calls,
            SuchThatType::ParentT,
            SuchThatType::CallsT,
            SuchThatType::Uses,
            SuchThatType::Modifies,
            SuchThatType::FollowsT,
            SuchThatType::Next,
            SuchThatType::NextT,
            SuchThatType::Affects,
            SuchThatType::AffectsT
    };
    return stRanks;
}

#include "ClauseScorer.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

long ClauseScorer::score(SuperClause *tc) {
    if (!tc->hasSynonyms() && !needsCFG(tc)) {
        return SCORE_CLAUSE_NO_SYNONYMS;
    } else if (tc->isWithClause()) {
        return scoreWithCl(tc);
    } else if (tc->isSuchThatClause()) {
        return scoreSuchThatCl(tc);
    } else if (tc->isPatternClause()) {
        return scorePatternCl(tc);
    }
    return 0;
}

// all with-clauses are treated the same, i.e assigned the same score
long ClauseScorer::scoreWithCl(SuperClause *cl) {
    if (!cl->isWithClause()) throw runtime_error("attempting to use with-clause scoring for non-with-clause");
    return SCORE_WITH_CLAUSE;
}

long ClauseScorer::scoreSuchThatCl(SuperClause *tc) {
    if (!tc->isSuchThatClause()) throw runtime_error("attempting to use suchthat-clause scoring for non-suchthat-clause");
    SuchThatType type = stClauseToType(tc);
    unordered_map<ClauseScorer::SuchThatType, long> ranks = getStRanks();
    long score = ranks[type];
    return score;
}

// all pattern-clauses are treated the same, i.e assigned the same score
long ClauseScorer::scorePatternCl(SuperClause *cl) {
    if (!cl->isPatternClause()) throw runtime_error("attempting to use pattern-clause scoring for non-pattern-clause");
    return SCORE_PATTERN_CLAUSE;
}

ClauseScorer::SuchThatType ClauseScorer::stClauseToType(SuperClause *tc) {
    if (!tc->isSuchThatClause()) throw runtime_error("cannot convert non-suchthat clause to its type");
    if (tc->isAffects()) return SuchThatType::Affects;
    if (tc->isAffectsT()) return SuchThatType::AffectsT;
    if (tc->isCalls()) return SuchThatType::Calls;
    if (tc->isCallsT()) return SuchThatType::CallsT;
    if (tc->isFollows()) return SuchThatType::Follows;
    if (tc->isFollowsT()) return SuchThatType::FollowsT;
    if (tc->isModifies()) return SuchThatType::Modifies;
    if (tc->isNext()) return SuchThatType::Next;
    if (tc->isNextT()) return SuchThatType::NextT;
    if (tc->isParent()) return SuchThatType::Parent;
    if (tc->isParentT()) return SuchThatType::ParentT;
    if (tc->isUses()) return SuchThatType::Uses;
    throw runtime_error("could not find matching for suchthat clause");
}

unordered_map<ClauseScorer::SuchThatType, long> ClauseScorer::getStRanks() {
    const static unordered_map<ClauseScorer::SuchThatType, long> stRanks = {
            {SuchThatType::Follows,  999},
            {SuchThatType::Next,     998},
            {SuchThatType::Calls,    850},
            {SuchThatType::Parent,   800},
            {SuchThatType::CallsT,   750},
            {SuchThatType::Modifies, 700},
            {SuchThatType::Uses,     650},
            {SuchThatType::ParentT,  600},
            {SuchThatType::FollowsT, 550},
            {SuchThatType::Affects,  30},
            {SuchThatType::NextT,    20},
            {SuchThatType::AffectsT, 10}
    };
    return stRanks;
}

bool ClauseScorer::needsCFG(SuperClause *tc) {
    return tc->isNext() || tc->isNextT() || tc->isAffects() || tc->isAffectsT();
}

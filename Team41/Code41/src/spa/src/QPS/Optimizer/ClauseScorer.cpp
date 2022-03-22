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
long ClauseScorer::scoreWithCl(SuperClause *tc) {
  return SCORE_WITH_CLAUSE;
}

long ClauseScorer::scoreSuchThatCl(SuperClause *tc) {
  SuchThatType type = stClauseToType(tc);
  unordered_map<ClauseScorer::SuchThatType, long> ranks = getStRanks();
  long idx = ranks[type];
  long piece = SCORE_SUCHTHAT_CLAUSE / (long)ranks.size();
  return SCORE_SUCHTHAT_CLAUSE - (idx * piece); // lower index = higher score
}

// all pattern-clauses are treated the same, i.e assigned the same score
long ClauseScorer::scorePatternCl(SuperClause *tc) {
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
  if (tc->isModifiesP() || tc->isModifiesS()) return SuchThatType::Modifies;
  if (tc->isNext()) return SuchThatType::Next;
  if (tc->isNextT()) return SuchThatType::NextT;
  if (tc->isParent()) return SuchThatType::Parent;
  if (tc->isParentT()) return SuchThatType::ParentT;
  if (tc->isUsesP() || tc->isUsesS()) return SuchThatType::Uses;
  throw runtime_error("could not find matching for suchthat clause");
}

unordered_map<ClauseScorer::SuchThatType, long> ClauseScorer::getStRanks() {
  static long rank = 1;
  const static unordered_map<ClauseScorer::SuchThatType, long> stRanks = {
	  {SuchThatType::Follows, rank++},
	  {SuchThatType::Parent, rank++},
	  {SuchThatType::Calls, rank++},
	  {SuchThatType::ParentT, rank++},
	  {SuchThatType::CallsT, rank++},
	  {SuchThatType::Uses, rank++},
	  {SuchThatType::Modifies, rank++},
	  {SuchThatType::FollowsT, rank++},
	  {SuchThatType::Next, rank++},
	  {SuchThatType::NextT, rank++},
	  {SuchThatType::Affects, rank++},
	  {SuchThatType::AffectsT, rank++}
  };
  return stRanks;
}

bool ClauseScorer::needsCFG(SuperClause *tc) {
  return tc->isNext() || tc->isNextT() || tc->isAffects() || tc->isAffectsT();
}

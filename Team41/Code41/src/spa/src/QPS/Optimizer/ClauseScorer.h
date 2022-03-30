#pragma once

#include "QPS/SuperClause.h"
#include "unordered_map"

/**
 * Clauses ranked from best (top, highest score) to worst (bottom, lowest score):
 * For suchthat-clauses, additional penalty is applied based on number of synonyms (more synonyms = lower score)
 *
 * 1. All clauses with no synonyms (except Next, Next*, Affects, Affects*, since those need CFG traversal)
 * 2. Follows
 * 3. Next
 * 4. All with-clauses
 * 5. Calls
 * 6. Parent
 * 7. Calls*
 * 8. Modifies
 * 9. Uses
 * 10. Parent*
 * 11. Follows*
 * 12. All pattern-clauses
 * 13. Affects
 * 14. Next*
 * 15. Affects*
 */
class ClauseScorer {
private:
    enum class SuchThatType {
        Follows, FollowsT, Parent, ParentT, Uses, Modifies, Calls, CallsT, Next, NextT, Affects, AffectsT
    };
    const static long SCORE_CLAUSE_NO_SYNONYMS = 1000;
    const static long SCORE_WITH_CLAUSE = 900;
    const static long SCORE_PATTERN_CLAUSE = 500;

    static unordered_map<SuchThatType, long> getStRanks();

    static SuchThatType stClauseToType(SuperClause *tc);

    static long scoreWithCl(SuperClause *tc);

    static long scoreSuchThatCl(SuperClause *tc);

    static long scorePatternCl(SuperClause *tc);

    static bool needsCFG(SuperClause *tc);

public:
    static long score(SuperClause *tc);
};

#pragma once

#include "QPS/SuperClause.h"
#include "unordered_map"

/**
 * Clauses ranked from best (top, highest score) to worst (bottom, lowest score):
 * For suchthat-clauses, additional penalty is applied based on number of synonyms (more synonyms = lower score)
 *
 * 1. All clauses with no synonyms (except Next, Next*, Affects, Affects*, since those need CFG traversal)
 * 2. All with-clauses
 * 3. All pattern-clauses
 * 4. Follows
 * 5. Parent
 * 6. Calls
 * 7. Parent*
 * 8. Calls*
 * 9. Uses
 * 10. Modifies
 * 11. Follows*
 * 12. Next
 * 13. Next*
 * 14. Affects
 * 15. Affects*
 */
class ClauseScorer {
private:
    enum class SuchThatType {
        Follows, FollowsT, Parent, ParentT, Uses, Modifies, Calls, CallsT, Next, NextT, Affects, AffectsT
    };
    const static long SCORE_CLAUSE_NO_SYNONYMS = 1000;
    const static long SCORE_WITH_CLAUSE = 700;
    const static long SCORE_PATTERN_CLAUSE = 500;
    const static long SCORE_SUCHTHAT_CLAUSE = 300;

    static unordered_map<SuchThatType, long> getStRanks();

    static SuchThatType stClauseToType(SuperClause *tc);

    static long scoreWithCl(SuperClause *tc);

    static long scoreSuchThatCl(SuperClause *tc);

    static long scorePatternCl(SuperClause *tc);

    static bool needsCFG(SuperClause *tc);

public:
    static long score(SuperClause *tc);
};

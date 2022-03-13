#pragma once

#include "TempClause.h"

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
    const static vector<SuchThatType> stRanks; // should not accessed directly, use getStRanks()

    static vector<SuchThatType> getStRanks();
    static SuchThatType stClauseToType(const TempClause &tc);
    static long scoreWithCl(const TempClause &tc);
    static long scoreSuchThatCl(const TempClause &tc);
    static long scorePatternCl(const TempClause &tc);
    static bool needsCFG(const TempClause &tc);
public:
    static long score(const TempClause &tc);
};

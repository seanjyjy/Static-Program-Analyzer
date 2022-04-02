#pragma once

#include "QPS/SuperClause.h"
#include "unordered_map"

/**
 * Assigns a numeric score to clauses, for sorting in optimization.
 * Clauses ranked from best (top, highest score) to worst (bottom, lowest score):
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

    /**
     * Returns a mapping of suchthat-clauses to their scores (clauses with larger scores are placed first).
     */
    static unordered_map<SuchThatType, long> getStRanks();

    /**
     * Given a suchthat-clause, returns a type recognisable by the Clause Scorer.
     * @throws runtime_error if the clause is not a suchthat-clause.
     */
    static SuchThatType stClauseToType(SuperClause *tc);

    /**
     * Returns a numeric score of the with-clause. Clauses with larger scores should be evaluated first.
     */
    static long scoreWithCl(SuperClause *tc);

    /**
     * Returns a numeric score of the suchthat-clause. Clauses with larger scores should be evaluated first.
     */
    static long scoreSuchThatCl(SuperClause *tc);

    /**
     * Returns a numeric score of the pattern-clause. Clauses with larger scores should be evaluated first.
     */
    static long scorePatternCl(SuperClause *tc);

    /**
     * Returns true if the clause requires evaluating with a Control Flow Graph, false otherwise.
     */
    static bool needsCFG(SuperClause *tc);

public:
    /**
     * Returns a numeric score of the given clause. Clauses with larger scores should be evaluated first.
     */
    static long score(SuperClause *tc);
};

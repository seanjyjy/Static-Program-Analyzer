#pragma once

#include "TempClause.h"

class ClauseScorer {
private:
    const static long SCORE_NO_SYNONYMS = 1000L;
    static long scoreWithCl(const TempClause &tc);
    static long scoreSuchThatCl(const TempClause &tc);
    static long scorePatternCl(const TempClause &tc);
public:
    static long score(const TempClause &tc);
};

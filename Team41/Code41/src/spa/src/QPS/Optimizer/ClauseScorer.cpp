#include "ClauseScorer.h"

long ClauseScorer::score(const TempClause &tc) {
    if (!tc.hasSynonyms()) return SCORE_NO_SYNONYMS;
    // TODO
    return 0;
}

long ClauseScorer::scoreWithCl(const TempClause &tc) {
    return 0;
}

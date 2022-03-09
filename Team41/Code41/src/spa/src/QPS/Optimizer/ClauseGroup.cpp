#include "ClauseGroup.h"
#include "ClauseScorer.h"

#include <functional>
#include <numeric>

using namespace std;

ClauseGroup::ClauseGroup() = default;

void ClauseGroup::addClause(const TempClause &c) {
    clauses.push_back(c);
}

long ClauseGroup::getScore() {
    return accumulate(clauses.begin(), clauses.end(), 0L, [](const TempClause &a, long currScore) {
        return ClauseScorer::score(a) + currScore;
    });
}


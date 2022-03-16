#include "ClauseGroup.h"
#include "ClauseScorer.h"

#include <algorithm>
#include <climits>
#include <functional>
#include <numeric>

using namespace std;

ClauseGroup::ClauseGroup(PKBAdapter pkbAdapter): table(TableEstimate(pkbAdapter)) {};

void ClauseGroup::addClause(const TempClause &c) {
    clauses.push_back(c);
    isUsed.push_back(false);
}

// the score of a clause group is just the sum of scores of its constituent clauses
long ClauseGroup::getScore() {
    return accumulate(clauses.begin(), clauses.end(), 0L, [](long currScore, TempClause &a) {
        return ClauseScorer::score(a) + currScore;
    });
}

void ClauseGroup::sortClauses() {
    sort(clauses.begin(), clauses.end(), [](TempClause &a, TempClause &b) {
        return ClauseScorer::score(a) < ClauseScorer::score(b);
    });
}

bool ClauseGroup::hasNextClause() {
    // TODO speedup with bit manipulation
    return any_of(isUsed.begin(), isUsed.end(), [](const bool used) { return !used; });
}

TempClause ClauseGroup::getNextClauseStatic() {
    // TODO speedup with bit manipulation
    for (int i = 0; i < isUsed.size(); i++) {
        if (!isUsed[i]) {
            isUsed[i] = true;
            table.merge(clauses[i].getSynonyms());
            return clauses[i];
        }
    }
    throw runtime_error("no more clauses left!");
}

TempClause ClauseGroup::getNextClauseDynamic() {
    // find the clause giving the smallest new intermediate table
    int bestIdx = -1;
    long long bestRows = LLONG_MAX;
    // TODO use priority queue?
    for (int i = 0; i < isUsed.size(); i++) {
        if (isUsed[i]) continue;
        long long rows = table.estimateMergeCost(clauses[i].getSynonyms());
        if (rows < bestRows) {
            bestRows = rows;
            bestIdx = i;
        }
    }
    // don't forget to update state
    if (bestIdx != -1) {
        isUsed[bestIdx] = true;
        return clauses[bestIdx];
    }
    throw runtime_error("no more clauses left");
}


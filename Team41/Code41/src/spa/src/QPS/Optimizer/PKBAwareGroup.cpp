#include "PKBAwareGroup.h"

#include <limits>

PKBAwareGroup::PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter) {
    table = TableEstimate(pkbAdapter);
    clauses = vector<SuperClause*>(initClauses.begin(), initClauses.end());
    isClauseUsed.assign(initClauses.size(), false);
    clauseScoreSum = accumulate(clauses.begin(), clauses.end(), 0L, [](long currScore, SuperClause *a) {
        return ClauseScorer::score(a) + currScore;
    });
}

// find the clause giving the smallest new intermediate table
SuperClause *PKBAwareGroup::pop() const {
    if (empty()) throw runtime_error("PKBAwareGroup: no more clauses to pop");

    int bestIdx = -1;
    long long bestRows = numeric_limits<long long>::max();
    for (int i = 0; i < (int) isClauseUsed.size(); i++) {
        if (!isClauseUsed[i] && table.estimateMergeCost(clauses[i]->getSynonyms()) < bestRows) {
            bestRows = table.estimateMergeCost(clauses[i]->getSynonyms());
            bestIdx = i;
        }
    }
    // don't forget to update state
    if (bestIdx == -1) throw runtime_error("incorrect assertion");
    isClauseUsed[bestIdx] = true;
    clausesPopped++;
    return clauses[bestIdx];
}

bool PKBAwareGroup::empty() const {
    return clausesPopped >= (long long) clauses.size();
}

size_t PKBAwareGroup::score() const {
    return clauseScoreSum;
}

void PKBAwareGroup::print() const {
    cout << toString() << endl;
}

string PKBAwareGroup::toString() const {
    string ret;
    ret += "\n***** PKB AWARE GROUP *****\n";
    for (SuperClause *cl: clauses) {
        ret += cl->toString() + "\n";
    }
    ret += "***************************";
    return ret;
}
#include "PKBAwareGroup.h"

#include <limits>
#include <utility>

PKBAwareGroup::PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter): PKBAwareGroup(move(initClauses), pkbAdapter, true) {
}

PKBAwareGroup::PKBAwareGroup(vector<SuperClause *> initClauses, PKBAdapter pkbAdapter, bool isEssential): ClauseGroup(isEssential) {
    table = TableEstimate(pkbAdapter);
    clauses = vector<SuperClause*>(initClauses.begin(), initClauses.end());
    isClauseUsed.assign(initClauses.size(), false);
    clauseScoreSum = accumulate(clauses.begin(), clauses.end(), 0L, [](long currScore, SuperClause *a) {
        return ClauseScorer::score(a) + currScore;
    });
}

SuperClause *PKBAwareGroup::pop() const {
    if (empty()) throw runtime_error("PKBAwareGroup: no more clauses to pop");

    // find the clause giving the smallest new intermediate table
    int bestIdx = -1;
    long long bestRows = numeric_limits<long long>::max();
    for (int i = 0; i < (int) isClauseUsed.size(); i++) {
        if (!isClauseUsed[i] && table.estimateMergeCost(clauses[i]->getSynonyms()) < bestRows) {
            bestRows = table.estimateMergeCost(clauses[i]->getSynonyms());
            bestIdx = i;
        }
    }

    // is there are no more clauses to pop, this method shouldn't even be called
    if (bestIdx == -1) throw runtime_error("incorrect assertion");

    // update table and group's state
    table.merge(clauses[bestIdx]->getSynonyms());
    isClauseUsed[bestIdx] = true;
    clausesPopped++;
    return clauses[bestIdx];
}

size_t PKBAwareGroup::size() const {
    return clauses.size();
}

bool PKBAwareGroup::empty() const {
    return clausesPopped >= (long long) clauses.size();
}

bool PKBAwareGroup::isLast() const {
    return clausesPopped == (long long) clauses.size()-1;
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

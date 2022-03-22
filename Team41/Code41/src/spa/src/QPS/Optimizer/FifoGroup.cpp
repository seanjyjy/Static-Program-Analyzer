#include "FifoGroup.h"

FifoGroup::FifoGroup(vector<SuperClause *> initClauses) {
    clauses = vector<SuperClause*>(initClauses.begin(), initClauses.end());
    idx = 0;
    clauseScoreSum = accumulate(clauses.begin(), clauses.end(), 0L, [](long currScore, SuperClause *a) {
        return ClauseScorer::score(a) + currScore;
    });
}

SuperClause *FifoGroup::pop() const {
    if (empty()) throw runtime_error("SortedGroup: no more clauses to pop");
    return clauses[idx++];
}

bool FifoGroup::empty() const {
    return idx >= (int) clauses.size();
}

size_t FifoGroup::score() const {
    return clauseScoreSum;
}

void FifoGroup::print() const {
    cout << toString() << endl;
}

string FifoGroup::toString() const {
    // TODO implement after superclause refactor
    string ret;
    ret += "\n***** FIFO GROUP *****\n";
    for ([[maybe_unused]] SuperClause *cl: clauses) {
        ret += "WIP\n";
    }
    ret += "\n**********************\n";
    return ret;
}

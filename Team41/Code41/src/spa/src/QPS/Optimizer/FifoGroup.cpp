#include "FifoGroup.h"

#include <utility>

FifoGroup::FifoGroup(vector<SuperClause *> clauses): FifoGroup(move(clauses), true) {
}

FifoGroup::FifoGroup(vector<SuperClause *> initClauses, bool isEssential): ClauseGroup(isEssential) {
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

size_t FifoGroup::size() const {
    return clauses.size();
}

bool FifoGroup::empty() const {
    return idx >= (int) clauses.size();
}

bool FifoGroup::isLast() const {
    return idx == (int) clauses.size()-1;
}

size_t FifoGroup::score() const {
    return clauseScoreSum;
}

void FifoGroup::print() const {
    cout << toString() << endl;
}

string FifoGroup::toString() const {
    string ret;
    ret += "\n***** FIFO GROUP *****\n";
    for (SuperClause *cl: clauses) {
        ret += cl->toString() + "\n";
    }
    ret += "\n**********************\n";
    return ret;
}

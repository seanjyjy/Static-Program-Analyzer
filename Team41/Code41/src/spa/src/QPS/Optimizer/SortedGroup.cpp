#include "SortedGroup.h"

#include <utility>

SortedGroup::SortedGroup(vector<SuperClause *> initClauses): SortedGroup(move(initClauses), true) {
}

SortedGroup::SortedGroup(vector<SuperClause *> initClauses, bool isEssential): ClauseGroup(isEssential) {
    clauses = multiset<SuperClause *, ClauseComparator>(initClauses.begin(), initClauses.end());
    it = clauses.begin();
    clauseScoreSum = accumulate(initClauses.begin(), initClauses.end(), 0L, [](long currScore, SuperClause *a) {
        return ClauseScorer::score(a) + currScore;
    });
}

SuperClause *SortedGroup::pop() const {
    if (empty()) throw runtime_error("SortedGroup: no more clauses to pop");
    return *(it++);
}

size_t SortedGroup::size() const {
    return clauses.size();
}

bool SortedGroup::empty() const {
    return it == clauses.end();
}

bool SortedGroup::isLast() const {
    return it != clauses.end() && next(it) == clauses.end();
}

size_t SortedGroup::score() const {
    return clauseScoreSum;
}

void SortedGroup::print() const {
    cout << toString() << endl;
}

string SortedGroup::toString() const {
    string ret;
    ret += "\n***** SORTED GROUP *****\n";
    for (SuperClause *cl: clauses) {
        ret += cl->toString() + "\n";
    }
    ret += "************************";
    return ret;
}

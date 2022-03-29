#include "FifoGroups.h"

#include <utility>

FifoGroups::FifoGroups(vector<ClauseGroup *> initGroups) {
    groups = vector<ClauseGroup*>(move(initGroups));
    it = groups.begin();
}

SuperClause *FifoGroups::pop() {
    if (empty()) throw runtime_error("no more clauses to pop");
    return (*it)->pop();
}

bool FifoGroups::empty() {
    // ensure invariant - first group always has a clause to take (unless
    while (it != groups.end() && (*it)->empty()) it++;
    return it == groups.end();
}

ClauseGroup *FifoGroups::front() {
    if (empty()) throw runtime_error("no more groups");
    return *it;
}

string FifoGroups::toString() const {
    string ret;
    for (ClauseGroup *a: groups) {
        ret += a->toString();
    }
    return ret;
}

void FifoGroups::print() const {
    cout << toString() << endl;
}

size_t FifoGroups::currGroupSize() const {
    return (*it)->size();
}

bool FifoGroups::isLastOfGroup() const {
    return (*it)->isLast();
}


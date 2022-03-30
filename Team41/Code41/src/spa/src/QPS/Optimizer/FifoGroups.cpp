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
    ensureInvariant();
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

size_t FifoGroups::currGroupSize() {
    ensureInvariant();
    return (*it)->size();
}

bool FifoGroups::isLastOfGroup() {
    ensureInvariant();
    return (*it)->isLast();
}

void FifoGroups::ensureInvariant() {
    // ensure invariant - first group always has a clause to take (unless no more groups)
    while (it != groups.end() && (*it)->empty()) it++;
}

bool FifoGroups::currGroupCanSimplify() {
    ensureInvariant();
    return (*it)->canSimplify();
}


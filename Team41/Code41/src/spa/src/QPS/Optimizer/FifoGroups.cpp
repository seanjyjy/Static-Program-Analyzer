#include "FifoGroups.h"

#include <utility>

FifoGroups::FifoGroups(vector<AbstractGroup *> initGroups) {
    groups = vector<AbstractGroup*>(move(initGroups));
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

AbstractGroup *FifoGroups::front() {
    if (empty()) throw runtime_error("no more groups");
    return *it;
}

string FifoGroups::toString() const {
    string ret;
    for (AbstractGroup *a: groups) {
        ret += a->toString();
    }
    return ret;
}

void FifoGroups::print() const {
    cout << toString() << endl;
}


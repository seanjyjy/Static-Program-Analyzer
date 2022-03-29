
#include "SortedGroups.h"

SortedGroups::SortedGroups(vector<ClauseGroup*> groups) {
    sortedGroups = multiset<ClauseGroup *, ClauseGroupComparator>(groups.begin(), groups.end());
    it = sortedGroups.begin();
}

SuperClause *SortedGroups::pop() {
    if (empty()) throw runtime_error("no more groups");
    return (*it)->pop();
}

bool SortedGroups::empty() {
    ensureInvariant();
    return it == sortedGroups.end();
}

ClauseGroup *SortedGroups::front() {
    if (empty()) throw runtime_error("no more groups");
    return *it;
}

string SortedGroups::toString() const {
    string ret;
    for (ClauseGroup *a: sortedGroups) {
        ret += a->toString();
    }
    return ret;
}

void SortedGroups::print() const {
    cout << toString() << endl;
}

size_t SortedGroups::currGroupSize() {
    ensureInvariant();
    return (*it)->size();
}

bool SortedGroups::isLastOfGroup() {
    ensureInvariant();
    return (*it)->isLast();
}

void SortedGroups::ensureInvariant() {
    // ensure invariant - first group always has a clause to take (unless all consumed)
    while (it != sortedGroups.end() && (*it)->empty()) it++;
}

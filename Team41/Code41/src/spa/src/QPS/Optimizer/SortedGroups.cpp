
#include "SortedGroups.h"

SortedGroups::SortedGroups(vector<ClauseGroup*> groups) {
    sortedGroups = multiset<ClauseGroup *, AbstractGroupComparator>(groups.begin(), groups.end());
    it = sortedGroups.begin();
}

SuperClause *SortedGroups::pop() {
    if (empty()) throw runtime_error("no more groups");
    return (*it)->pop();
}

bool SortedGroups::empty() {
    // ensure invariant - first group always has a clause to take
    while (it != sortedGroups.end() && (*it)->empty()) it = next(it);
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

size_t SortedGroups::currGroupSize() const {
    return (*it)->size();
}

bool SortedGroups::isLastOfGroup() const {
    return (*it)->isLast();
}

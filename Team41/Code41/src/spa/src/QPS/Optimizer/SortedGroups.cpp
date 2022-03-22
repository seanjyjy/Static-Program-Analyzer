
#include "SortedGroups.h"

SortedGroups::SortedGroups() {
    it = sortedGroups.begin();
}

void SortedGroups::addGroup(AbstractGroup *group) {
    sortedGroups.insert(group);
}

SuperClause *SortedGroups::pop() {
    if (empty()) throw runtime_error("no more groups");
    return (*it)->pop();
}

bool SortedGroups::empty() {
    // ensure invariant - first group always has a clause to take (unless
    while (it != sortedGroups.end() && front()->empty()) it++;
    return it == sortedGroups.end();
}

AbstractGroup *SortedGroups::front() {
    if (empty()) throw runtime_error("no more groups");
    return *it;
}

string SortedGroups::toString() const {
    string ret;
    for (AbstractGroup *a: sortedGroups) {
        ret += a->toString();
    }
    return ret;
}

void SortedGroups::print() const {
    cout << toString() << endl;
}


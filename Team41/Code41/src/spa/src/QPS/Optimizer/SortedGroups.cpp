
#include "SortedGroups.h"

SortedGroups::SortedGroups() {
  it = sortedGroups.begin();
};

void SortedGroups::addGroup(const AbstractGroup &group) {
  sortedGroups.insert(group);
}
SuperClause *SortedGroups::pop() {
  if (empty()) throw runtime_error("no more groups");
  return it->pop();
}
bool SortedGroups::empty() {
  // ensure invariant - first group always has a clause to take (unless
  while (it != sortedGroups.end() && front().empty()) it++;
  return it == sortedGroups.end();
}
const AbstractGroup &SortedGroups::front() {
  if (empty()) throw runtime_error("no more groups");
  return *it;
}


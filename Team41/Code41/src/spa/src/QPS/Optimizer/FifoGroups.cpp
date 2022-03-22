#include "FifoGroups.h"

FifoGroups::FifoGroups() {
  it = groups.begin();
}
void FifoGroups::addGroup(const AbstractGroup &group) {
  groups.push_back(group);
}
SuperClause *FifoGroups::pop() {
  if (empty()) throw runtime_error("no more clauses to pop");
  return it->pop();
}
bool FifoGroups::empty() {
  // ensure invariant - first group always has a clause to take (unless
  while (it != groups.end() && front().empty()) it++;
  return it == groups.end();
}
const AbstractGroup &FifoGroups::front() {
  if (empty()) throw runtime_error("no more groups");
  return *it;
};


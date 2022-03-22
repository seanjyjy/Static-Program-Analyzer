#include "ClauseGroups.h"
#include <stdexcept>
#include <algorithm>

using namespace std;

ClauseGroups::ClauseGroups() = default;

ClauseGroups::ClauseGroups(PKBAdapter pkbAdapter) : pkbAdapter(pkbAdapter) {};

ClauseGroups::ClauseGroups(int n, PKBAdapter pkbAdapter) : pkbAdapter(pkbAdapter) {
  groups.assign(n, ClauseGroup(pkbAdapter));
}

void ClauseGroups::sortGroups() {
  sort(groups.begin(), groups.end(), [](ClauseGroup &a, ClauseGroup &b) {
	return a.getScore() < b.getScore();
  });
}

void ClauseGroups::sortEachGroup() {
  for (ClauseGroup &cg : groups) {
	cg.sortClauses();
  }
}

void ClauseGroups::addClause(int idx, SuperClause *clause) {
  if (idx < 0 || idx >= groups.size()) throw out_of_range("invalid index " + to_string(idx));
  groups[idx].addClause(clause);
}

void ClauseGroups::addClauseGroup(const ClauseGroup &cg) {
  groups.push_back(cg);
}

bool ClauseGroups::hasNextGroup() {
  return groupsIdx < groups.size();
}

ClauseGroup ClauseGroups::getNextGroup() {
  return groups.at(groupsIdx++);
}

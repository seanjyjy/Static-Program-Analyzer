#include <stdexcept>
#include <algorithm>

using namespace std;

#include "ClauseGroups.h"
#include "TempClause.h"

ClauseGroups::ClauseGroups() = default;

ClauseGroups::ClauseGroups(int n) {
    groups.assign(n, ClauseGroup());
}

void ClauseGroups::sortGroups() {
    sort(groups.begin(), groups.end(), [](const ClauseGroup &a, const ClauseGroup &b) {
       return a.getScore() < b.getScore();
    });
}

void ClauseGroups::sortEachGroup() {
    // TODO
}

void ClauseGroups::addClause(int idx, const TempClause &clause) {
    if (idx < 0 || idx >= groups.size()) throw out_of_range("invalid index " + to_string(idx));
    groups[idx].addClause(clause);
}

void ClauseGroups::addClauseGroup(const ClauseGroup &cg) {
    groups.push_back(cg);
}

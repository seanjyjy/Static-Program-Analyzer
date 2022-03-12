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
    sort(groups.begin(), groups.end(), [](ClauseGroup &a, ClauseGroup &b) {
       return a.getScore() < b.getScore();
    });
}

void ClauseGroups::sortEachGroup() {
    for (const ClauseGroup &cg: groups) {
        cg.sortClauses();
    }
}

void ClauseGroups::addClause(int idx, const TempClause &clause) {
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

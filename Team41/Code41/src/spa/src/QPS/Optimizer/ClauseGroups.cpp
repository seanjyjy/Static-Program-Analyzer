#include <stdexcept>
#include <algorithm>
#include <utility>

using namespace std;

#include "ClauseGroups.h"
#include "TempClause.h"

ClauseGroups::ClauseGroups(PKBAdapter pkbAdapter): pkbAdapter(std::move(pkbAdapter)) {}

ClauseGroups::ClauseGroups(int n, PKBAdapter pkbAdapter): pkbAdapter(std::move(pkbAdapter)) {
    groups.assign(n, ClauseGroup(pkbAdapter));
}

void ClauseGroups::sortGroups() {
    sort(groups.begin(), groups.end(), [](ClauseGroup &a, ClauseGroup &b) {
       return a.getScore() < b.getScore();
    });
}

void ClauseGroups::sortEachGroup() {
    for (ClauseGroup &cg: groups) {
        cg.sortClauses();
    }
}

void ClauseGroups::addClause(int idx, const TempClause &clause) {
    if (idx < 0 || idx >= (int) groups.size()) throw out_of_range("invalid index " + to_string(idx));
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

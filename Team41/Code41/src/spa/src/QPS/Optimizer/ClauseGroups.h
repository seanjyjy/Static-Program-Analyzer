#pragma once

#include <vector>

using namespace std;

#include "ClauseGroup.h"
#include "TempClause.h"

class ClauseGroups {
private:
    vector<ClauseGroup> groups;
public:
    ClauseGroups();
    ClauseGroups(int n);
    void sortGroups();
    void sortEachGroup();
    void addClause(int idx, const TempClause &clause);
    void addClauseGroup(const ClauseGroup &cg);
};
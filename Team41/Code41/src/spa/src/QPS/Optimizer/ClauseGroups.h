#pragma once

#include <vector>

using namespace std;

#include "ClauseGroup.h"
#include "TempClause.h"

class ClauseGroups {
private:
    PKBAdapter pkbAdapter;
    vector<ClauseGroup> groups;
    size_t groupsIdx = 0;
public:
    ClauseGroups(PKBAdapter adapter);
    ClauseGroups(int n, PKBAdapter adapter);
    void sortGroups();
    void sortEachGroup();
    void addClause(int idx, const TempClause &clause);
    void addClauseGroup(const ClauseGroup &cg);
    bool hasNextGroup();
    ClauseGroup getNextGroup();
};
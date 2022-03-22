#pragma once

#include <vector>

using namespace std;

#include "ClauseGroup.h"
#include "QPS/SuperClause.h"

class ClauseGroups {
private:
    PKBAdapter pkbAdapter;
    vector<ClauseGroup> groups;
    size_t groupsIdx = 0;
public:
    ClauseGroups();

    ClauseGroups(PKBAdapter adapter);

    ClauseGroups(int n, PKBAdapter adapter);

    void sortGroups();

    void sortEachGroup();

    void addClause(int idx, SuperClause *clause);

    void addClauseGroup(const ClauseGroup &cg);

    bool hasNextGroup();

    ClauseGroup getNextGroup();
};
#pragma once

#include <vector>

using namespace std;

#include "TempClause.h"

class ClauseGroup {
private:
    vector<TempClause> clauses;
public:
    ClauseGroup();
    void addClause(const TempClause &c);
    long getScore();
};

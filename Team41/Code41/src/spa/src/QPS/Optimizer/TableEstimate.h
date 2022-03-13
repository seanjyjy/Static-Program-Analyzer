#pragma once

#include <vector>
#include <string>
#include "QPS/ClauseVariable.h"
#include "QPS/Optimizer/PKBAdapter.h"

using namespace std;

struct cvHasher {
    size_t operator() (const ClauseVariable &cv) const {
        size_t a = hash<int>{}(cv.getType());
        size_t b = hash<string>{}(cv.getLabel());
        return a ^ (b << 1);
    }
};

struct cvEquals {
    size_t operator() (const ClauseVariable &a, const ClauseVariable &b) const {
        if (a.getType() == b.getType() && a.getLabel() == b.getLabel()) return true;
        return false;
    }
};

class TableEstimate {
private:
    PKBAdapter pkbAdapter;
    vector<ClauseVariable> schema;
    long long estRows;

    bool hasCommonCol(const vector<ClauseVariable>& sch);
public:

    TableEstimate(PKBAdapter &pkbAdapter);
    long long estimateMergeCost(const vector<ClauseVariable> &sch);
    void merge(const vector<ClauseVariable> &sch);

    // getters and setters
    long long getEstimatedRows();
    vector<ClauseVariable> getSchema();
};

#pragma once

#include <vector>
#include <string>
#include "QPS/ClauseVariable.h"
#include "QPS/Optimizer/PKBAdapter.h"

using namespace std;

struct cvHasher {
    size_t operator() (const QueryDeclaration &cv) const {
        size_t a = hash<int>{}(cv.getType());
        size_t b = hash<string>{}(cv.getSynonym());
        return a ^ (b << 1);
    }
};

struct cvEquals {
    size_t operator() (const QueryDeclaration &a, const QueryDeclaration &b) const {
        if (a.getType() == b.getType() && a.getSynonym() == b.getSynonym()) return true;
        return false;
    }
};

class TableEstimate {
private:
    PKBAdapter pkbAdapter;
    vector<QueryDeclaration> schema;
    long long estRows;

    bool hasCommonCol(const vector<QueryDeclaration>& sch);
public:
    TableEstimate();
    TableEstimate(PKBAdapter &pkbAdapter);
    long long estimateMergeCost(const vector<QueryDeclaration> &sch);
    void merge(const vector<QueryDeclaration> &sch);

    // getters and setters
    long long getEstimatedRows();
    vector<QueryDeclaration> getSchema();
};

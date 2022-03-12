#include "TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"

#include <algorithm>
#include <iterator>
#include <unordered_set>

using namespace std;

TableEstimate::TableEstimate(PKBAdapter &pkbAdapter): pkbAdapter(pkbAdapter) {}

void TableEstimate::merge(const vector<ClauseVariable> &sch) {
    // merge with new schema
    unordered_set<ClauseVariable, cvHasher> a(schema.begin(), schema.end());
    for (const ClauseVariable &cv: sch) {
        if (a.find(cv) == a.end()) schema.push_back(cv);
    }

    // update row estimate
    estRows = estimateMergeCost(sch);
}

bool TableEstimate::hasCommonCol(const vector<ClauseVariable> &sch) {
    unordered_set<ClauseVariable, cvHasher> a(schema.begin(), schema.end());
    for (const ClauseVariable &cv: sch) {
        if (a.find(cv) != a.end()) return true;
    }
    return false;
}

long long TableEstimate::getEstimatedRows() {
    return estRows;
}

vector<ClauseVariable> TableEstimate::getSchema() {
    return schema;
}

long long TableEstimate::estimateMergeCost(const vector<ClauseVariable> &newSch) {
    if (hasCommonCol(newSch)) {
        return max(estRows, pkbAdapter.getRowCount(newSch));
    } else {
        return estRows * pkbAdapter.getRowCount(newSch);
    }
}

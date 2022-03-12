#include "PKBAdapter.h"

PKBAdapter::PKBAdapter(PKBManager pkb): pkb(move(pkb)) {
}

long long PKBAdapter::getRowCount(const ClauseVariable &synonym) {
    // TODO stub
    return 0;
}

long long PKBAdapter::getRowCount(const vector<ClauseVariable> &synonyms) {
    long long rows = 0;
    for (const ClauseVariable &cv: synonyms) rows *= getRowCount(cv);
    return rows;
}

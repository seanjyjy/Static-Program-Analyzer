#include "PKBAdapter.h"

#include <utility>

PKBAdapter::PKBAdapter() = default;

PKBAdapter::PKBAdapter(PKBManager *pkbMgr) : pkb(pkbMgr) {
}

long long PKBAdapter::getRowCount(const QueryDeclaration &synonym) {
    EntitiesReader *reader = synonym.getType()->getReader();
    return reader->getRowcount(pkb);
}

long long PKBAdapter::getRowCount(const vector<QueryDeclaration> &synonyms) {
    long long rows = 1;
    for (const QueryDeclaration &cv: synonyms) {
        rows = rows * getRowCount(cv);
    }
    return rows;
}

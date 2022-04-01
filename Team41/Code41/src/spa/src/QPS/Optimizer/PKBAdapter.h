#pragma once

#include "PKB/PKBManager.h"
#include "QPS/ClauseVariable.h"

/**
 * Implementation of the Adapter design pattern over a PKB Manager.
 * Mainly used by the query optimizer to estimate the number of rows from evaluating a clause.
 */
class PKBAdapter {
private:
    // the underlying pkb interface
    PKBManager *pkb;
public:
    PKBAdapter();

    PKBAdapter(PKBManager *pkbMgr);

    /**
     * Returns an estimate of the number of rows the PKB has for a given synonym.
     */
    long long getRowCount(const QueryDeclaration &synonym);

    /**
     * Returns an estimate of the number of rows the PKB has for a table with columns from the given list of synonyms.
     */
    long long getRowCount(const vector<QueryDeclaration> &synonyms);
};

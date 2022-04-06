#pragma once

#include <vector>
#include <string>
#include "QPS/QO/ClauseVariable.h"
#include "QPS/Optimizer/PKBAdapter.h"

using namespace std;

/**
 * Custom hashing function for query declaration.
 */
struct queryDeclarationHasher {
    size_t operator()(const QueryDeclaration &cv) const {
        size_t a = hash<int>{}(cv.getType()->hashCode());
        size_t b = hash<string>{}(cv.getSynonym());
        return a ^ (b << 1);
    }
};

/**
 * Custom equals function for query declaration.
 */
struct queryDeclarationEquals {
    size_t operator()(const QueryDeclaration &a, const QueryDeclaration &b) const {
        if (a.getType()->toString() == b.getType()->toString() && a.getSynonym() == b.getSynonym()) return true;
        return false;
    }
};

/**
 * Represents an estimate of a table with columns as synonyms, and a number of rows.
 * Used by groups that need to query the PKB (e.g PKBAwareGroup) to keep an estimate on how many rows have been
 * evaluated without having to actually evaluate the clauses and generate the table.
 */
class TableEstimate {
private:
    PKBAdapter pkbAdapter;
    vector<QueryDeclaration> schema; // columns of the table
    long long estRows = 0; // number of rows of the table

    // returns true if the given schema has overlapping synonyms with the current schema
    bool hasCommonCol(const vector<QueryDeclaration> &sch);
public:
    // do NOT use this in business logic - only meant for default class member initialization
    TableEstimate();

    /**
     * Creates a table estimate with access to the PKB through the PKBAdapter.
     */
    TableEstimate(const PKBAdapter &pkbAdapter);

    /**
     * Returns an estimate on the number of rows in the table after merging with a table of the given schema.
     */
    long long estimateMergeCost(const vector<QueryDeclaration> &sch);

    /**
     * Merges the given table with the current table, updating internal table columns and row count.
     */
    void merge(const vector<QueryDeclaration> &sch);

    // getters and setters
    [[nodiscard]] long long getEstimatedRows() const;

    [[nodiscard]] vector<QueryDeclaration> getSchema() const;
};

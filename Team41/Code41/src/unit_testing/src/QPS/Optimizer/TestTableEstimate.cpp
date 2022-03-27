#include "catch.hpp"
#include "QPS/Optimizer/TableEstimate.h"
#include "TestOptimizerUtils.h"
#include "PKB/PKBManager.h"

TEST_CASE("Table Estimate") {
    // variant of the week 7 optimization lecture example
    int stmtCount = 5;
    int varCount = 3;

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    for (int i = 0; i < stmtCount; i++) pkbManager.registerAssignStmt(to_string(i));
    for (int i = 0; i < varCount; i++) pkbManager.registerVariable(to_string(i));

    PKBAdapter adapter(&pkbManager);
    TableEstimate tableEstimate(adapter);

    // first merge - s3 v1
    string s11 = "s3";
    string s12 = "v1";
    vector<QueryDeclaration> first = {
            {QueryDeclaration::STMT, s11},
            {QueryDeclaration::VARIABLE, s12},
    };

    // verify post-merge state is correct
    REQUIRE(tableEstimate.estimateMergeCost(first) == stmtCount*varCount);
    tableEstimate.merge(first);
    REQUIRE(tableEstimate.getEstimatedRows() == stmtCount*varCount);

    TestOptimizerUtils::print(tableEstimate.getSchema());

    // second merge - s3 "x" - ignore constant
    string s21 = "s3";
    vector<QueryDeclaration> second = {
            {QueryDeclaration::STMT, s21},
    };

    // merge is constrained by the table with less rows
    REQUIRE(tableEstimate.estimateMergeCost(second) == stmtCount);
    tableEstimate.merge(second);
    REQUIRE(tableEstimate.getEstimatedRows() == stmtCount);

    // third merge - s1, s2
    string s31 = "s1";
    string s32 = "s2";
    vector<QueryDeclaration> third = {
            {QueryDeclaration::STMT, s31},
            {QueryDeclaration::STMT, s32},
    };

    // merge is a cross-product, no common rows
    int rows = (int) tableEstimate.getEstimatedRows();
    REQUIRE(tableEstimate.estimateMergeCost(third) == rows*stmtCount*stmtCount);
    tableEstimate.merge(third);
    REQUIRE(tableEstimate.getEstimatedRows() == rows*stmtCount*stmtCount);
}
//
// Created by JinHao on 26/1/22.
//

#include "catch.hpp"
#include "PKB/EntityTable.h"

using namespace std;

set<string> EMPTY_SET;

TEST_CASE("EntityTable: constant") {
    EntityTable table;

    set<string> expectedConstant;
    // Before adding
    REQUIRE(table.getConstants() == expectedConstant);
    REQUIRE(table.getVariables() == EMPTY_SET);
    REQUIRE(table.getProcedures() == EMPTY_SET);

    // add 1 value
    REQUIRE_NOTHROW(table.addConstant("abc"));
    expectedConstant.insert("abc");
    REQUIRE(table.getConstants() == expectedConstant);

    // add repeated value
    REQUIRE_NOTHROW(table.addConstant("abc"));
    REQUIRE(table.getConstants() == expectedConstant);

    // add another value
    REQUIRE_NOTHROW(table.addConstant("ABC"));
    expectedConstant.insert("ABC");
    REQUIRE(table.getConstants() == expectedConstant);

    // other entities not affected
    REQUIRE(table.getVariables() == EMPTY_SET);
    REQUIRE(table.getProcedures() == EMPTY_SET);
}

TEST_CASE("EntityTable: adding variables") {
    EntityTable table;

    set<string> expectedVariable;
    // Before adding
    REQUIRE(table.getConstants() == EMPTY_SET);
    REQUIRE(table.getVariables() == expectedVariable);
    REQUIRE(table.getProcedures() == EMPTY_SET);

    // add 1 value
    REQUIRE_NOTHROW(table.addVariable("abc"));
    expectedVariable.insert("abc");
    REQUIRE(table.getVariables() == expectedVariable);

    // add repeated value
    REQUIRE_NOTHROW(table.addVariable("abc"));
    REQUIRE(table.getVariables() == expectedVariable);

    // add another value
    REQUIRE_NOTHROW(table.addVariable("ABC"));
    expectedVariable.insert("ABC");
    REQUIRE(table.getVariables() == expectedVariable);

    // other entities not affected
    REQUIRE(table.getConstants() == EMPTY_SET);
    REQUIRE(table.getProcedures() == EMPTY_SET);
}

TEST_CASE("EntityTable: adding procedures") {
    EntityTable table;

    set<string> expectedProcedure;
    // Before adding
    REQUIRE(table.getConstants() == EMPTY_SET);
    REQUIRE(table.getVariables() == EMPTY_SET);
    REQUIRE(table.getProcedures() == expectedProcedure);

    // add 1 value
    REQUIRE_NOTHROW(table.addProcedure("abc"));
    expectedProcedure.insert("abc");
    REQUIRE(table.getProcedures() == expectedProcedure);

    // add repeated value
    REQUIRE_NOTHROW(table.addProcedure("abc"));
    REQUIRE(table.getProcedures() == expectedProcedure);

    // add another value
    REQUIRE_NOTHROW(table.addProcedure("ABC"));
    expectedProcedure.insert("ABC");
    REQUIRE(table.getProcedures() == expectedProcedure);

    // other entities not affected
    REQUIRE(table.getConstants() == EMPTY_SET);
    REQUIRE(table.getVariables() == EMPTY_SET);
}
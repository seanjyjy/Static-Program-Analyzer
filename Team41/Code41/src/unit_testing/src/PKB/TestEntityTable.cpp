#include "catch.hpp"
#include "PKB/EntityTable.h"

using namespace std;

TEST_CASE("EntityTable") {
    EntityTable table;
    set<string> EMPTY_SET;

    SECTION("empty state") {
        REQUIRE(table.getConstants() == EMPTY_SET);
        REQUIRE(table.getVariables() == EMPTY_SET);
        REQUIRE(table.getProcedures() == EMPTY_SET);
    }

    SECTION("adding constant") {
        set<string> expectedConstant;

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

    SECTION("adding variables") {
        set<string> expectedVariable;

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

    SECTION("adding procedures") {
        set<string> expectedProcedure;

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
}

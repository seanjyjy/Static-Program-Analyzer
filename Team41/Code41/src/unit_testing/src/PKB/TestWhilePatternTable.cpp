#include "catch.hpp"
#include "PKB/Tables/WhilePatternTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: WhilePatternTable") {
    WhilePatternTable table;
    string stmt[] = {"s0", "s1", "s2"};
    string vars[] = {"v0", "v1", "v2"};

    SECTION("Initial State") {
        REQUIRE(table.getStmtFromVar(vars[0]).empty());
        REQUIRE(table.getStmtNVar().empty());
        REQUIRE(table.getStmtsUsingSomeVarInPred().empty());
    }

    SECTION("Basic functionality") {
        vector<pair<string, string>> entryList;

        REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[0]));
        REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[1]));
        REQUIRE_NOTHROW(table.setPattern(stmt[1], vars[1]));
        REQUIRE_NOTHROW(table.setPattern(stmt[2], vars[2]));
        entryList.push_back({stmt[0], vars[0]});
        entryList.push_back({stmt[0], vars[1]});
        entryList.push_back({stmt[1], vars[1]});
        entryList.push_back({stmt[2], vars[2]});

        REQUIRE(table.getStmtFromVar(vars[0]) == unordered_set<string>({stmt[0]}));
        REQUIRE(table.getStmtFromVar(vars[1]) == unordered_set<string>({stmt[0], stmt[1]}));
        REQUIRE(table.getStmtFromVar(vars[2]) == unordered_set<string>({stmt[2]}));
        REQUIRE(sortAndCompareVectors(table.getStmtNVar(), entryList));
        REQUIRE(table.getStmtsUsingSomeVarInPred() == unordered_set<string>({stmt[0], stmt[1], stmt[2]}));
    }
}

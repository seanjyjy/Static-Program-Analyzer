#include "catch.hpp"
#include "PKB/ParentTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("ParentTable") {
    ParentTable table;
    string stmt[] = {"0", "1", "2", "3", "4", "5"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("Parent") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getParentEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getParentOf(stmt[0]) == "");
            REQUIRE(table.getAllChildrenOf(stmt[0]) == EMPTY_SET);

            REQUIRE_FALSE(table.isParent(stmt[1], stmt[2]));
            REQUIRE_FALSE(table.isParent(stmt[1], stmt[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // 0 -> {1, 2}
            // 1 -> 5
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[1]));
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[2]));
            REQUIRE_NOTHROW(table.setParent(stmt[1], stmt[5]));
            entryList.push_back(make_pair(stmt[0], stmt[1]));
            entryList.push_back(make_pair(stmt[0], stmt[2]));
            entryList.push_back(make_pair(stmt[1], stmt[5]));

            REQUIRE(sortAndCompareVectors(table.getParentEntries(), entryList));

            // 0 -> {1, 2}
            // 1 -> 5
            REQUIRE(table.getParentOf(stmt[0]) == "");
            REQUIRE(table.getParentOf(stmt[1]) == stmt[0]);
            REQUIRE(table.getParentOf(stmt[2]) == stmt[0]);
            REQUIRE(table.getParentOf(stmt[5]) == stmt[1]);

            // 0 -> {1, 2}
            // 1 -> 5
            REQUIRE(table.getAllChildrenOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2]}));
            REQUIRE(table.getAllChildrenOf(stmt[1]) == unordered_set<string>({stmt[5]}));
            REQUIRE(table.getAllChildrenOf(stmt[3]) == unordered_set<string>());

            REQUIRE(table.isParent(stmt[0], stmt[1]));
            REQUIRE_FALSE(table.isParent(stmt[0], stmt[5]));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            REQUIRE_THROWS_WITH(table.setParent(stmt[0], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[ParentTable]") &&
                                Catch::Contains("cannot parent self"));

            // 0 -> 5
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[5]));
            // repeated follow does not throw error
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[5]));


            // 0 -> 5
            // 4 -x-> 5
            REQUIRE_THROWS_WITH(table.setParent(stmt[4], stmt[5]),
                                Catch::Contains("[PKB]") && Catch::Contains("[Parent]") &&
                                Catch::Contains("[One-Many]") && Catch::Contains("Multiple keys"));
        }
    }

    SECTION("ParentT") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getParentTEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getAllAncestorsOf(stmt[0]) == EMPTY_SET);
            REQUIRE(table.getAllDescendantsOf(stmt[0]) == EMPTY_SET);

            REQUIRE_FALSE(table.isParentT(stmt[1], stmt[2]));
            REQUIRE_FALSE(table.isParentT(stmt[1], stmt[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // 0 -> {1, 2}
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[1]));
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[2]));
            entryList.push_back(make_pair(stmt[0], stmt[1]));
            entryList.push_back(make_pair(stmt[0], stmt[2]));
            REQUIRE(table.getAllDescendantsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2]}));
            REQUIRE(table.getAllAncestorsOf(stmt[1]) == unordered_set<string>({stmt[0]}));
            REQUIRE(sortAndCompareVectors(table.getParentTEntries(), entryList));
            REQUIRE(table.isParentT(stmt[0], stmt[1]));

            // 0 -> {1, 2}
            // 2 -> 3
            REQUIRE_NOTHROW(table.setParent(stmt[2], stmt[3]));
            entryList.push_back(make_pair(stmt[2], stmt[3]));
            entryList.push_back(make_pair(stmt[0], stmt[3]));
            REQUIRE(table.getAllDescendantsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2], stmt[3]}));
            REQUIRE(table.getAllDescendantsOf(stmt[1]) == unordered_set<string>());
            REQUIRE(table.getAllDescendantsOf(stmt[2]) == unordered_set<string>({stmt[3]}));
            REQUIRE(table.getAllAncestorsOf(stmt[3]) == unordered_set<string>({stmt[0], stmt[2]}));
            REQUIRE(sortAndCompareVectors(table.getParentTEntries(), entryList));
            REQUIRE(table.isParentT(stmt[0], stmt[3]));
            REQUIRE_FALSE(table.isParentT(stmt[1], stmt[3]));

            // 0 -> {1, 2}
            // 2 -> 3
            // 3 -> {4, 5}
            REQUIRE_NOTHROW(table.setParent(stmt[3], stmt[4]));
            REQUIRE_NOTHROW(table.setParent(stmt[3], stmt[5]));
            entryList.push_back(make_pair(stmt[0], stmt[4]));
            entryList.push_back(make_pair(stmt[0], stmt[5]));
            entryList.push_back(make_pair(stmt[2], stmt[4]));
            entryList.push_back(make_pair(stmt[2], stmt[5]));
            entryList.push_back(make_pair(stmt[3], stmt[4]));
            entryList.push_back(make_pair(stmt[3], stmt[5]));

            REQUIRE(table.getAllDescendantsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2], stmt[3], stmt[4], stmt[5]}));
            REQUIRE(table.getAllDescendantsOf(stmt[1]) == unordered_set<string>());
            REQUIRE(table.getAllDescendantsOf(stmt[2]) == unordered_set<string>({stmt[3], stmt[4], stmt[5]}));
            REQUIRE(table.getAllDescendantsOf(stmt[3]) == unordered_set<string>({stmt[4], stmt[5]}));

            REQUIRE(table.getAllAncestorsOf(stmt[3]) == unordered_set<string>({stmt[0], stmt[2]}));
            REQUIRE(table.getAllAncestorsOf(stmt[4]) == unordered_set<string>({stmt[0], stmt[2], stmt[3]}));
            REQUIRE(table.getAllAncestorsOf(stmt[5]) == unordered_set<string>({stmt[0], stmt[2], stmt[3]}));

            REQUIRE(sortAndCompareVectors(table.getParentTEntries(), entryList));
            REQUIRE(table.isParentT(stmt[0], stmt[5]));
            REQUIRE_FALSE(table.isParentT(stmt[1], stmt[5]));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            // 0 -> 1
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[1]));
            // repeated follow does not throw error
            REQUIRE_NOTHROW(table.setParent(stmt[0], stmt[1]));

            // 0 -> 1
            // 1 -x-> 0
            REQUIRE_THROWS_WITH(table.setParent(stmt[1], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[ParentTable]") &&
                                Catch::Contains("Cyclic dependency"));

            // 0 -> 1 -> 2 -> 3
            REQUIRE_NOTHROW(table.setParent(stmt[1], stmt[2]));
            REQUIRE_NOTHROW(table.setParent(stmt[2], stmt[3]));

            // 0 -> 1 -> 2 -> 3 -x-> 0
            REQUIRE_THROWS_WITH(table.setParent(stmt[3], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[ParentTable]") &&
                                Catch::Contains("Cyclic dependency"));
        }
    }
}

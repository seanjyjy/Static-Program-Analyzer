#include "catch.hpp"
#include "PKB/FollowsTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("FollowsTable") {
    FollowsTable table;
    string stmt[] = {"0", "1", "2", "3", "4", "5"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("Follows") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getFollowEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getStmtFollowedBy(stmt[0]) == "");
            REQUIRE(table.getStmtFollowing(stmt[0]) == "");

            REQUIRE_FALSE(table.isFollows(stmt[1], stmt[2]));
            REQUIRE_FALSE(table.isFollows(stmt[1], stmt[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // 0 -> 1 -> 2 -> 5
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[1]));
            REQUIRE_NOTHROW(table.setFollows(stmt[1], stmt[2]));
            REQUIRE_NOTHROW(table.setFollows(stmt[2], stmt[5]));
            entryList.push_back(make_pair(stmt[0], stmt[1]));
            entryList.push_back(make_pair(stmt[1], stmt[2]));
            entryList.push_back(make_pair(stmt[2], stmt[5]));

            REQUIRE(sortAndCompareVectors(table.getFollowEntries(), entryList));

            // 0 -> 1 -> 2 -> 5
            REQUIRE(table.getStmtFollowedBy(stmt[0]) == stmt[1]);
            REQUIRE(table.getStmtFollowedBy(stmt[1]) == stmt[2]);
            REQUIRE(table.getStmtFollowedBy(stmt[2]) == stmt[5]);
            REQUIRE(table.getStmtFollowedBy(stmt[5]) == "");

            // 0 <- 1 <- 2 <- 5
            REQUIRE(table.getStmtFollowing(stmt[0]) == "");
            REQUIRE(table.getStmtFollowing(stmt[1]) == stmt[0]);
            REQUIRE(table.getStmtFollowing(stmt[2]) == stmt[1]);
            REQUIRE(table.getStmtFollowing(stmt[5]) == stmt[2]);

            REQUIRE(table.isFollows(stmt[0], stmt[1]));
            REQUIRE_FALSE(table.isFollows(stmt[0], stmt[5]));

            REQUIRE(table.getStmtsFollowingSomeStmt() == unordered_set<string>({stmt[0], stmt[1], stmt[2]}));
            REQUIRE(table.getStmtsFollowedBySomeStmt() == unordered_set<string>({stmt[1], stmt[2], stmt[5]}));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            REQUIRE_THROWS_WITH(table.setFollows(stmt[0], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[FollowsTable]") &&
                                Catch::Contains("cannot follow self"));

            // 0 -> 5
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[5]));
            // repeated follow does not throw error
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[5]));

            // 0 -x-> 1
            REQUIRE_THROWS_WITH(table.setFollows(stmt[0], stmt[1]),
                                Catch::Contains("[PKB]") && Catch::Contains("[Follows]") &&
                                Catch::Contains("[One-One]") && Catch::Contains("Key already mapped"));

            // 4 -x-> 5
            REQUIRE_THROWS_WITH(table.setFollows(stmt[4], stmt[5]),
                                Catch::Contains("[PKB]") && Catch::Contains("[Follows]") &&
                                Catch::Contains("[One-One]") && Catch::Contains("Value already mapped"));
        }
    }

    SECTION("FollowsT") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getFollowTEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getStmtsFollowedTBy(stmt[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsFollowingT(stmt[0]) == EMPTY_SET);

            REQUIRE_FALSE(table.isFollowsT(stmt[1], stmt[2]));
            REQUIRE_FALSE(table.isFollowsT(stmt[1], stmt[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // 0 -> 1
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[1]));
            entryList.push_back(make_pair(stmt[0], stmt[1]));
            REQUIRE(table.getStmtsFollowedTBy(stmt[0]) == unordered_set<string>({stmt[1]}));
            REQUIRE(table.getStmtsFollowingT(stmt[1]) == unordered_set<string>({stmt[0]}));
            REQUIRE(sortAndCompareVectors(table.getFollowTEntries(), entryList));
            REQUIRE(table.isFollowsT(stmt[0], stmt[1]));

            // 2 -> 3
            REQUIRE_NOTHROW(table.setFollows(stmt[2], stmt[3]));
            entryList.push_back(make_pair(stmt[2], stmt[3]));
            REQUIRE(table.getStmtsFollowedTBy(stmt[2]) == unordered_set<string>({stmt[3]}));
            REQUIRE(table.getStmtsFollowingT(stmt[3]) == unordered_set<string>({stmt[2]}));
            REQUIRE(sortAndCompareVectors(table.getFollowTEntries(), entryList));
            REQUIRE(table.isFollowsT(stmt[2], stmt[3]));

            //  0 -> 1 -> 2 -> 3
            REQUIRE_NOTHROW(table.setFollows(stmt[1], stmt[2]));
            entryList.push_back(make_pair(stmt[0], stmt[2]));
            entryList.push_back(make_pair(stmt[0], stmt[3]));
            entryList.push_back(make_pair(stmt[1], stmt[2]));
            entryList.push_back(make_pair(stmt[1], stmt[3]));

            REQUIRE(table.getStmtsFollowedTBy(stmt[0]) == unordered_set<string>({stmt[1], stmt[2], stmt[3]}));
            REQUIRE(table.getStmtsFollowedTBy(stmt[1]) == unordered_set<string>({stmt[2], stmt[3]}));
            REQUIRE(table.getStmtsFollowedTBy(stmt[2]) == unordered_set<string>({stmt[3]}));
            REQUIRE(table.getStmtsFollowedTBy(stmt[3]) == unordered_set<string>());

            REQUIRE(table.getStmtsFollowingT(stmt[0]) == unordered_set<string>());
            REQUIRE(table.getStmtsFollowingT(stmt[1]) == unordered_set<string>({stmt[0]}));
            REQUIRE(table.getStmtsFollowingT(stmt[2]) == unordered_set<string>({stmt[0], stmt[1]}));
            REQUIRE(table.getStmtsFollowingT(stmt[3]) == unordered_set<string>({stmt[0], stmt[1], stmt[2]}));

            REQUIRE(sortAndCompareVectors(table.getFollowTEntries(), entryList));
            REQUIRE(table.isFollowsT(stmt[1], stmt[2]));
            REQUIRE(table.isFollowsT(stmt[0], stmt[3]));
            REQUIRE(table.isFollowsT(stmt[0], stmt[2]));
            REQUIRE(table.isFollowsT(stmt[1], stmt[3]));

            REQUIRE(table.getStmtsFollowingSomeStmt() == unordered_set<string>({stmt[0], stmt[1], stmt[2]}));
            REQUIRE(table.getStmtsFollowedBySomeStmt() == unordered_set<string>({stmt[1], stmt[2], stmt[3]}));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            // 0 -> 1
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[1]));
            // repeated follow does not throw error
            REQUIRE_NOTHROW(table.setFollows(stmt[0], stmt[1]));

            // 0 -> 1 -x-> 0
            REQUIRE_THROWS_WITH(table.setFollows(stmt[1], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[FollowsTable]") &&
                                Catch::Contains("Cyclic dependency"));

            // 0 -> 1 -> 2 -> 3
            REQUIRE_NOTHROW(table.setFollows(stmt[1], stmt[2]));
            REQUIRE_NOTHROW(table.setFollows(stmt[2], stmt[3]));

            // 0 -> 1 -> 2 -> 3 -x-> 0
            REQUIRE_THROWS_WITH(table.setFollows(stmt[3], stmt[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[FollowsTable]") &&
                                Catch::Contains("Cyclic dependency"));
        }
    }
}

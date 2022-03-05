#include "catch.hpp"
#include "PKB/Tables/CallsTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: CallsTable") {
    CallsTable table;
    string proc[] = {"p0", "p1", "p2", "p3", "p4", "p5"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("Calls") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getCallsEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getProcsCalling(proc[0]) == EMPTY_SET);
            REQUIRE(table.getProcsCalledBy(proc[0]) == EMPTY_SET);

            REQUIRE_FALSE(table.isCalls(proc[1], proc[2]));
            REQUIRE_FALSE(table.isCalls(proc[1], proc[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // p0 -> {p1, p2}
            // p2 -> p5
            REQUIRE_NOTHROW(table.setCalls(proc[0], proc[1]));
            REQUIRE_NOTHROW(table.setCalls(proc[0], proc[2]));
            REQUIRE_NOTHROW(table.setCalls(proc[2], proc[5]));
            entryList.push_back(make_pair(proc[0], proc[1]));
            entryList.push_back(make_pair(proc[0], proc[2]));
            entryList.push_back(make_pair(proc[2], proc[5]));

            REQUIRE(sortAndCompareVectors(table.getCallsEntries(), entryList));
            REQUIRE(table.getProcsCalledBy(proc[0]) == unordered_set<string>({proc[1], proc[2]}));
            REQUIRE(table.getProcsCalledBy(proc[2]) == unordered_set<string>({proc[5]}));
            REQUIRE(table.getProcsCalling(proc[0]) == unordered_set<string>());
            REQUIRE(table.getProcsCalling(proc[1]) == unordered_set<string>({proc[0]}));
            REQUIRE(table.getProcsCalling(proc[2]) == unordered_set<string>({proc[0]}));
            REQUIRE(table.getProcsCalling(proc[5]) == unordered_set<string>({proc[2]}));

            REQUIRE(table.getProcsCallingSomeProc() == unordered_set<string>({proc[0], proc[2]}));
            REQUIRE(table.getProcsCalledBySomeProc() == unordered_set<string>({proc[1], proc[2], proc[5]}));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            REQUIRE_THROWS_WITH(table.setCalls(proc[0], proc[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[CallsTable]") &&
                                Catch::Contains("Recursion is not allowed"));

            // 0 -> 5
            REQUIRE_NOTHROW(table.setCalls(proc[0], proc[5]));
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[5]));
            // repeated calls does not throw error
            REQUIRE_NOTHROW(table.setCalls(proc[0], proc[5]));

            REQUIRE_THROWS_WITH(table.setCalls(proc[5], proc[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[CallsTable]") &&
                                Catch::Contains("Cyclic dependency"));
        }
    }

    SECTION("CallsT") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getCallsTEntries(), EMPTY_SET_PAIR));
            REQUIRE(table.getProcsCallingT(proc[0]) == EMPTY_SET);
            REQUIRE(table.getProcsCalledTBy(proc[0]) == EMPTY_SET);

            REQUIRE_FALSE(table.isCallsT(proc[1], proc[2]));
            REQUIRE_FALSE(table.isCallsT(proc[1], proc[1]));
        }

        SECTION("Basic functionality") {
            vector<pair<string, string>> entryList;

            // p0 -> {p1, p2}
            // p2 -> p3
            // p3 -> p4
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[1]));
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[2]));
            REQUIRE_NOTHROW(table.setCallsT(proc[2], proc[3]));
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[3]));
            REQUIRE_NOTHROW(table.setCallsT(proc[3], proc[4]));
            REQUIRE_NOTHROW(table.setCallsT(proc[2], proc[4]));
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[4]));
            entryList.push_back(make_pair(proc[0], proc[1]));
            entryList.push_back(make_pair(proc[0], proc[2]));
            entryList.push_back(make_pair(proc[0], proc[3]));
            entryList.push_back(make_pair(proc[0], proc[4]));
            entryList.push_back(make_pair(proc[2], proc[3]));
            entryList.push_back(make_pair(proc[2], proc[4]));
            entryList.push_back(make_pair(proc[3], proc[4]));

            REQUIRE(sortAndCompareVectors(table.getCallsTEntries(), entryList));
            REQUIRE(table.getProcsCalledTBy(proc[0]) == unordered_set<string>({proc[1], proc[2], proc[3], proc[4]}));
            REQUIRE(table.getProcsCalledTBy(proc[1]) == unordered_set<string>());
            REQUIRE(table.getProcsCalledTBy(proc[2]) == unordered_set<string>({proc[3], proc[4]}));
            REQUIRE(table.getProcsCalledTBy(proc[3]) == unordered_set<string>({proc[4]}));
            REQUIRE(table.getProcsCallingT(proc[4]) == unordered_set<string>({proc[0], proc[2], proc[3]}));
        }

            // domain specific edge cases
        SECTION("Edge cases") {
            // 0 -..-> 1
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[1]));
            // repeated calls does not throw error
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[1]));

            // 0 -..-> 1 -x-> 0
            REQUIRE_THROWS_WITH(table.setCallsT(proc[1], proc[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[CallsTable]") &&
                                Catch::Contains("Cyclic dependency"));

            // 0 -..-> 1 -..-> 2 -..-> 3
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[2]));
            REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[3]));
            REQUIRE_NOTHROW(table.setCallsT(proc[1], proc[2]));
            REQUIRE_NOTHROW(table.setCallsT(proc[1], proc[3]));
            REQUIRE_NOTHROW(table.setCallsT(proc[2], proc[3]));

            // 0 -> 1 -> 2 -> 3 -x-> 0
            REQUIRE_THROWS_WITH(table.setCallsT(proc[3], proc[0]),
                                Catch::Contains("[PKB]") && Catch::Contains("[CallsTable]") &&
                                Catch::Contains("Cyclic dependency"));
        }
    }

    SECTION("Combined") {
        vector<pair<string, string>> callsList;
        vector<pair<string, string>> callsTList;

        // 0 -> {1, 2}
        // 1 -> {2, 3}
        REQUIRE_NOTHROW(table.setCalls(proc[0], proc[1]));
        REQUIRE_NOTHROW(table.setCalls(proc[0], proc[2]));
        REQUIRE_NOTHROW(table.setCalls(proc[1], proc[2]));
        REQUIRE_NOTHROW(table.setCalls(proc[1], proc[3]));
        REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[1]));
        REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[2]));
        REQUIRE_NOTHROW(table.setCallsT(proc[1], proc[2]));
        REQUIRE_NOTHROW(table.setCallsT(proc[1], proc[3]));
        REQUIRE_NOTHROW(table.setCallsT(proc[0], proc[3]));
        callsList.push_back(make_pair(proc[0], proc[1]));
        callsList.push_back(make_pair(proc[0], proc[2]));
        callsList.push_back(make_pair(proc[1], proc[2]));
        callsList.push_back(make_pair(proc[1], proc[3]));
        callsTList.push_back(make_pair(proc[0], proc[1]));
        callsTList.push_back(make_pair(proc[0], proc[2]));
        callsTList.push_back(make_pair(proc[1], proc[2]));
        callsTList.push_back(make_pair(proc[1], proc[3]));
        callsTList.push_back(make_pair(proc[0], proc[3]));
        REQUIRE(sortAndCompareVectors(table.getCallsEntries(), callsList));
        REQUIRE(sortAndCompareVectors(table.getCallsTEntries(), callsTList));
        REQUIRE(table.getProcsCalledBy(proc[0]) == unordered_set<string>({proc[1], proc[2]}));
        REQUIRE(table.getProcsCalledTBy(proc[0]) == unordered_set<string>({proc[1], proc[2], proc[3]}));
        REQUIRE(table.getProcsCalling(proc[3]) == unordered_set<string>({proc[1]}));
        REQUIRE(table.getProcsCallingT(proc[3]) == unordered_set<string>({proc[0], proc[1]}));
        REQUIRE(table.isCalls(proc[0], proc[1]));
        REQUIRE_FALSE(table.isCalls(proc[0], proc[3]));
        REQUIRE(table.isCallsT(proc[0], proc[3]));
        REQUIRE_FALSE(table.isCallsT(proc[2], proc[3]));
    }
}

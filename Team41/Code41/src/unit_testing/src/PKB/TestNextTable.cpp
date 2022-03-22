#include "catch.hpp"
#include "PKB/Tables/NextTable.h"
#include "Common/CFGBuilder.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: NextTable") {
    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3"}},
            {"3",  {"4", "7"}},
            {"4",  {"5"}},
            {"5",  {"6"}},
            {"6",  {"3"}},
            {"7",  {"8", "9"}},
            {"8",  {"10"}},
            {"9",  {"10"}},
            {"10", {"11"}},
            {"11", {"12"}},
            {"12", {}},
            {"13", {}}
    };
    string startStmt = "1";
    string endStmt = "13";
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {startStmt, endStmt});
    cfgBuilder.build();
    CFGNode *rootNode = cfgBuilder.getCFG();
    unordered_map<string, CFGNode *> stmtToCFG = cfgBuilder.getStmtNumToNodeMap();
    NextTable table;

    SECTION("Next") {
        SECTION("Initial State") {
            REQUIRE(table.getAllNext().empty());
            REQUIRE(table.getNextNodes(startStmt).empty());
            REQUIRE(table.getNextNodes(endStmt).empty());
            REQUIRE(table.getPrevNodes(startStmt).empty());
            REQUIRE(table.getPrevNodes(endStmt).empty());
            REQUIRE(table.getAllStmtsExecBeforeSomeStmt().empty());
            REQUIRE(table.getAllStmtsExecAfterSomeStmt().empty());
            REQUIRE(table.getNumOfEndNodes() == 0);
            REQUIRE(table.getNumOfStartNodes() == 0);
            REQUIRE_FALSE(table.isNext(startStmt, endStmt));
            REQUIRE_FALSE(table.isNext(endStmt, startStmt));
            REQUIRE_FALSE(table.isNext(startStmt, startStmt));
            REQUIRE_FALSE(table.isNext(endStmt, endStmt));
        }

        table.setCFGNode(rootNode, stmtToCFG);;
        SECTION("isNext") {
            for (auto[from, next]: stmtNextMap) {
                for (int j = 0; j <= 14; j++) { // non existing stmt number was used to test edge cases
                    string to = to_string(j);
                    if (find(next.begin(), next.end(), to) != next.end()) {
                        REQUIRE(table.isNext(from, to));
                    } else {
                        REQUIRE_FALSE(table.isNext(from, to));
                    }
                }
            }
        }

        SECTION("getNextNodes") {
            for (auto[from, next]: stmtNextMap) {
                vector<CFGNode *> nextNodes;
                for (const string &nextStmt: next) {
                    nextNodes.push_back(stmtToCFG[nextStmt]);
                }
                REQUIRE(sortAndCompareVectors(table.getNextNodes(from), nextNodes));
            }
        }

        SECTION("getAllNext") {
            vector<pair<string, string >> expected;
            for (auto[from, next]: stmtNextMap) {
                for (const auto &to: next) {
                    expected.push_back({from, to});
                }
            }
            REQUIRE(sortAndCompareVectors(table.getAllNext(), expected));
        }

        SECTION("getAllStmtsExecBeforeSomeStmt") {
            vector<string> expected;
            for (auto[from, next]: stmtNextMap) {
                if (!next.empty()) {
                    expected.push_back(from);
                }
            }
            REQUIRE(sortAndCompareVectors(table.getAllStmtsExecBeforeSomeStmt(), expected));
            REQUIRE(table.getNumOfStartNodes() == expected.size());
        }


        unordered_map<string, vector<string>> stmtPrevMap;
        for (auto[from, next]: stmtNextMap) {
            for (const auto &to: next) {
                stmtPrevMap[to].push_back(from);
            }
        }
        SECTION("getPrevNodes") {
            for (auto[to, prev]: stmtPrevMap) {
                vector<CFGNode *> prevNodes;
                for (const string &from: prev) {
                    prevNodes.push_back(stmtToCFG[from]);
                }
                REQUIRE(sortAndCompareVectors(table.getPrevNodes(to), prevNodes));
            }
        }

        SECTION("getAllStmtsExecAfterSomeStmt") {
            vector<string> expected;
            for (auto[to, prev]: stmtPrevMap) {
                if (!prev.empty()) {
                    expected.push_back(to);
                }
            }
            REQUIRE(sortAndCompareVectors(table.getAllStmtsExecAfterSomeStmt(), expected));
            REQUIRE(table.getNumOfEndNodes() == expected.size());
        }
    }
}

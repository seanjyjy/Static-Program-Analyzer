#include "catch.hpp"
#include "PKB/UsesTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("UsesTable") {
    UsesTable table;
    string vars[] = {"var1", "var2"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("UsesS") {
        string lines[] = {"1", "2"};
        SECTION("Initial State") {
            REQUIRE(table.getVarsUsedInStmt(lines[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsUsingVar(vars[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsVarEntries() == EMPTY_SET_PAIR);
            REQUIRE_FALSE(table.isUsesS(lines[0], vars[0]));
        }

        SECTION("Basic functionality") {
            unordered_set<string> stmtList;
            unordered_set<string> varList;
            vector<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarUsedInStmt(lines[0], vars[0]));
            entryList.push_back(make_pair(lines[0], vars[0]));
            stmtList.insert(lines[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsUsedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsUsingVar(vars[0]) == stmtList);
            REQUIRE(compareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isUsesS(lines[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarUsedInStmt(lines[0], vars[1]));
            entryList.push_back(make_pair(lines[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsUsedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsUsingVar(vars[1]) == stmtList);
            REQUIRE(compareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isUsesS(lines[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarUsedInStmt(lines[1], vars[1]));
            entryList.push_back(make_pair(lines[1], vars[1]));
            stmtList.insert(lines[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsUsedInStmt(lines[1]) == varList);
            REQUIRE(table.getStmtsUsingVar(vars[1]) == stmtList);
            REQUIRE(compareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isUsesS(lines[1], vars[1]));
        }
    }
    SECTION("UsesP") {
        string proc[] = {"proc1", "proc2"};

        SECTION("Initial State") {
            REQUIRE(table.getVarsUsedInProc(proc[0]) == EMPTY_SET);
            REQUIRE(table.getProcsUsingVar(vars[0]) == EMPTY_SET);
            REQUIRE(table.getProcVarEntries() == EMPTY_SET_PAIR);
            REQUIRE_FALSE(table.isUsesP(proc[0], vars[0]));
        }

        SECTION("Basic functionality") {
            unordered_set<string> procList;
            unordered_set<string> varList;
            vector<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarUsedInProc(proc[0], vars[0]));
            entryList.push_back(make_pair(proc[0], vars[0]));
            procList.insert(proc[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsUsedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsUsingVar(vars[0]) == procList);
            REQUIRE(compareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isUsesP(proc[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarUsedInProc(proc[0], vars[1]));
            entryList.push_back(make_pair(proc[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsUsedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsUsingVar(vars[1]) == procList);
            REQUIRE(compareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isUsesP(proc[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarUsedInProc(proc[1], vars[1]));
            entryList.push_back(make_pair(proc[1], vars[1]));
            procList.insert(proc[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsUsedInProc(proc[1]) == varList);
            REQUIRE(table.getProcsUsingVar(vars[1]) == procList);
            REQUIRE(compareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isUsesP(proc[1], vars[1]));
        }
    }
}

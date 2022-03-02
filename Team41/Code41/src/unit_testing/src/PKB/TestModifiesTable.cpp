#include "catch.hpp"
#include "PKB/Tables/ModifiesTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: ModifiesTable") {
    ModifiesTable table;
    string vars[] = {"var1", "var2"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("ModifiesS") {
        string lines[] = {"1", "2"};
        SECTION("Initial State") {
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsModifyingVar(vars[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsVarEntries() == EMPTY_SET_PAIR);
            REQUIRE_FALSE(table.isModifiesS(lines[0], vars[0]));
        }

        SECTION("Basic functionality") {
            unordered_set<string> stmtList;
            unordered_set<string> varList;
            vector<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[0], vars[0]));
            entryList.push_back(make_pair(lines[0], vars[0]));
            stmtList.insert(lines[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[0]) == stmtList);
            REQUIRE(sortAndCompareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isModifiesS(lines[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[0], vars[1]));
            entryList.push_back(make_pair(lines[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[1]) == stmtList);
            REQUIRE(sortAndCompareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isModifiesS(lines[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[1], vars[1]));
            entryList.push_back(make_pair(lines[1], vars[1]));
            stmtList.insert(lines[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInStmt(lines[1]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[1]) == stmtList);
            REQUIRE(sortAndCompareVectors(table.getStmtsVarEntries(), entryList));
            REQUIRE(table.isModifiesS(lines[1], vars[1]));

            REQUIRE(table.getStmtsModifyingSomeVar() == unordered_set<string>({lines[0], lines[1]}));
            REQUIRE(table.getVarsModifiedInSomeStmt() == unordered_set<string>({vars[0], vars[1]}));
        }
    }
    SECTION("ModifiesP") {
        string proc[] = {"proc1", "proc2"};

        SECTION("Initial State") {
            REQUIRE(table.getVarsModifiedInProc(proc[0]) == EMPTY_SET);
            REQUIRE(table.getProcsModifyingVar(vars[0]) == EMPTY_SET);
            REQUIRE(table.getProcVarEntries() == EMPTY_SET_PAIR);
            REQUIRE_FALSE(table.isModifiesP(proc[0], vars[0]));
        }

        SECTION("Basic functionality") {
            unordered_set<string> procList;
            unordered_set<string> varList;
            vector<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[0], vars[0]));
            entryList.push_back(make_pair(proc[0], vars[0]));
            procList.insert(proc[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsModifiedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[0]) == procList);
            REQUIRE(sortAndCompareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isModifiesP(proc[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[0], vars[1]));
            entryList.push_back(make_pair(proc[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[1]) == procList);
            REQUIRE(sortAndCompareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isModifiesP(proc[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[1], vars[1]));
            entryList.push_back(make_pair(proc[1], vars[1]));
            procList.insert(proc[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInProc(proc[1]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[1]) == procList);
            REQUIRE(sortAndCompareVectors(table.getProcVarEntries(), entryList));
            REQUIRE(table.isModifiesP(proc[1], vars[1]));

            REQUIRE(table.getProcsModifyingSomeVar() == unordered_set<string>({proc[0], proc[1]}));
            REQUIRE(table.getVarsModifiedInSomeProc() == unordered_set<string>({vars[0], vars[1]}));
        }
    }
}

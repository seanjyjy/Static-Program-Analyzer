#include "catch.hpp"
#include "PKB/ModifiesTable.h"

using namespace std;

TEST_CASE("ModifiesTable") {
    ModifiesTable table;
    string vars[] = {"var1", "var2"};
    set<string> EMPTY_SET;
    set<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("ModifiesS") {
        string lines[] = {"1", "2"};
        SECTION("Initial State") {
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsModifyingVar(vars[0]) == EMPTY_SET);
            REQUIRE(table.getStmtsVarEntries() == EMPTY_SET_PAIR);
            REQUIRE_FALSE(table.isModifiesS(lines[0], vars[0]));
        }

        SECTION("Basic functionality") {
            set<string> stmtList;
            set<string> varList;
            set<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[0], vars[0]));
            entryList.insert(make_pair(lines[0], vars[0]));
            stmtList.insert(lines[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[0]) == stmtList);
            REQUIRE(table.getStmtsVarEntries() == entryList);
            REQUIRE(table.isModifiesS(lines[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[0], vars[1]));
            entryList.insert(make_pair(lines[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInStmt(lines[0]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[1]) == stmtList);
            REQUIRE(table.getStmtsVarEntries() == entryList);
            REQUIRE(table.isModifiesS(lines[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarModifiedInStmt(lines[1], vars[1]));
            entryList.insert(make_pair(lines[1], vars[1]));
            stmtList.insert(lines[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInStmt(lines[1]) == varList);
            REQUIRE(table.getStmtsModifyingVar(vars[1]) == stmtList);
            REQUIRE(table.getStmtsVarEntries() == entryList);
            REQUIRE(table.isModifiesS(lines[1], vars[1]));
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
            set<string> procList;
            set<string> varList;
            set<pair<string, string>> entryList;

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[0], vars[0]));
            entryList.insert(make_pair(proc[0], vars[0]));
            procList.insert(proc[0]);
            varList.insert(vars[0]);
            REQUIRE(table.getVarsModifiedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[0]) == procList);
            REQUIRE(table.getProcVarEntries() == entryList);
            REQUIRE(table.isModifiesP(proc[0], vars[0]));

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[0], vars[1]));
            entryList.insert(make_pair(proc[0], vars[1]));
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInProc(proc[0]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[1]) == procList);
            REQUIRE(table.getProcVarEntries() == entryList);
            REQUIRE(table.isModifiesP(proc[0], vars[1]));

            REQUIRE_NOTHROW(table.setVarModifiedInProc(proc[1], vars[1]));
            entryList.insert(make_pair(proc[1], vars[1]));
            procList.insert(proc[1]);
            varList.clear();
            varList.insert(vars[1]);
            REQUIRE(table.getVarsModifiedInProc(proc[1]) == varList);
            REQUIRE(table.getProcsModifyingVar(vars[1]) == procList);
            REQUIRE(table.getProcVarEntries() == entryList);
            REQUIRE(table.isModifiesP(proc[1], vars[1]));
        }
    }
}

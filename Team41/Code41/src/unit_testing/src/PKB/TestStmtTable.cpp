#include "catch.hpp"
#include "PKB/StmtTable.h"

using namespace std;

TEST_CASE("StmtTable") {
    string lines[] = {"1", "2", "3", "4", "5", "6"};
    StmtType types[] = {ASSIGN, CALL, IF, PRINT, READ,
                        WHILE};
    StmtTable table;

    SECTION("empty table") {
        // Before adding
        REQUIRE(table.getStmtCount() == 0);
        REQUIRE(table.getAllStmtsByType(ASSIGN) == unordered_set<string>());
        REQUIRE(table.getAllStmts() == unordered_set<string>());
        REQUIRE(table.getAllTypes() == unordered_set<StmtType>());
        REQUIRE_FALSE(table.isStmtType(lines[0], ASSIGN));
    }

    SECTION("adding statements") {
        // Adding key-values
        unordered_set<StmtType> stmtSet;
        int i = 0;
        for (StmtType t: types) {
            table.setStmt(lines[i], t);
            stmtSet.insert(t);
            i++;
        }
        REQUIRE(table.getAllTypes() == stmtSet);

        unordered_set<string> stmtNumbers;
        for (string l: lines) {
            stmtNumbers.insert(l);
        }
        REQUIRE(table.getAllStmts() == stmtNumbers);

        // throw error if statement has multiple types
        REQUIRE_THROWS(table.setStmt(lines[0], WHILE));
        // don't throw if it's just a repeat
        REQUIRE_NOTHROW(table.setStmt(lines[0], ASSIGN));

        // query
        REQUIRE(table.isStmtType(lines[0], ASSIGN));
        REQUIRE_FALSE(table.isStmtType(lines[0], WHILE));
        REQUIRE(table.isStmtType(lines[5], WHILE));
        REQUIRE(table.getStmtCount() == 6);

        // assign --> {"1", "7"}
        REQUIRE_NOTHROW(table.setStmt("7", ASSIGN));
        REQUIRE(table.getAllStmtsByType(ASSIGN).size() == 2);
        unordered_set<string> assignSet;
        assignSet.insert("1");
        assignSet.insert("7");
        REQUIRE(table.getAllStmtsByType(ASSIGN) == assignSet);
        REQUIRE(table.getStmtCount() == 7);
    }
}

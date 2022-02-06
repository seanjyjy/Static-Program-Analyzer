#include "catch.hpp"
#include "PKB/PKB.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: entities") {
    unordered_set<string> EMPTY_SET;
    string entity[] = {"abc", "def"};
    unordered_set<string> FILLED_SET_1;
    FILLED_SET_1.insert(entity[0]);
    unordered_set<string> FILLED_SET_2;
    FILLED_SET_2.insert(entity[0]);
    FILLED_SET_2.insert(entity[1]);

    PKB pkbManager;

    SECTION("add variables") {
        REQUIRE(pkbManager.getVariables() == EMPTY_SET);
        REQUIRE_NOTHROW(pkbManager.registerVariable(entity[0]));
        REQUIRE(pkbManager.getVariables() == FILLED_SET_1);
        REQUIRE_NOTHROW(pkbManager.registerVariable(entity[1]));
        REQUIRE(pkbManager.getVariables() == FILLED_SET_2);
        // check that other table are not affected
        REQUIRE(pkbManager.getConstants() == EMPTY_SET);
    }

    SECTION("add constants") {
        REQUIRE(pkbManager.getConstants() == EMPTY_SET);
        REQUIRE_NOTHROW(pkbManager.registerConstant(entity[0]));
        REQUIRE(pkbManager.getConstants() == FILLED_SET_1);
        REQUIRE_NOTHROW(pkbManager.registerConstant(entity[1]));
        REQUIRE(pkbManager.getConstants() == FILLED_SET_2);
    }

    SECTION("add procedure") {
        REQUIRE(pkbManager.getProcedures() == EMPTY_SET);
        REQUIRE_NOTHROW(pkbManager.registerProcedure(entity[0]));
        REQUIRE(pkbManager.getProcedures() == FILLED_SET_1);
        REQUIRE_NOTHROW(pkbManager.registerProcedure(entity[1]));
        REQUIRE(pkbManager.getProcedures() == FILLED_SET_2);
    }
}

TEST_CASE("PKB: statements") {
    unordered_set<string> EMPTY_SET;
    string stmt[] = {"1", "2", "3", "4"};
    unordered_set<string> FILLED_SET_1;
    FILLED_SET_1.insert(stmt[0]);
    unordered_set<string> FILLED_SET_2;
    FILLED_SET_2.insert(stmt[0]);
    FILLED_SET_2.insert(stmt[1]);

    PKB pkbManager;

    SECTION("add assign") {
        REQUIRE(pkbManager.getAssigns() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isAssignStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerAssign(stmt[0]));
        REQUIRE(pkbManager.isAssignStmt(stmt[0]));
        REQUIRE(pkbManager.getAssigns() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isAssignStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerAssign(stmt[1]));
        REQUIRE(pkbManager.isAssignStmt(stmt[1]));
        REQUIRE(pkbManager.getAssigns() == FILLED_SET_2);

        // check that other table are not affected
        REQUIRE(pkbManager.getCalls() == EMPTY_SET);
    }

    SECTION("add while") {
        REQUIRE(pkbManager.getWhiles() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isWhileStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerWhile(stmt[0]));
        REQUIRE(pkbManager.isWhileStmt(stmt[0]));
        REQUIRE(pkbManager.getWhiles() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isWhileStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerWhile(stmt[1]));
        REQUIRE(pkbManager.isWhileStmt(stmt[1]));
        REQUIRE(pkbManager.getWhiles() == FILLED_SET_2);
    }

    SECTION("add ifs") {
        REQUIRE(pkbManager.getIfs() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isIfStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerIf(stmt[0]));
        REQUIRE(pkbManager.isIfStmt(stmt[0]));
        REQUIRE(pkbManager.getIfs() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isIfStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerIf(stmt[1]));
        REQUIRE(pkbManager.isIfStmt(stmt[1]));
        REQUIRE(pkbManager.getIfs() == FILLED_SET_2);
    }

    SECTION("add reads") {
        REQUIRE(pkbManager.getReads() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isReadStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerRead(stmt[0]));
        REQUIRE(pkbManager.isReadStmt(stmt[0]));
        REQUIRE(pkbManager.getReads() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isReadStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerRead(stmt[1]));
        REQUIRE(pkbManager.isReadStmt(stmt[1]));
        REQUIRE(pkbManager.getReads() == FILLED_SET_2);
    }

    SECTION("add prints") {
        REQUIRE(pkbManager.getPrints() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isPrintStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerPrint(stmt[0]));
        REQUIRE(pkbManager.isPrintStmt(stmt[0]));
        REQUIRE(pkbManager.getPrints() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isPrintStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerPrint(stmt[1]));
        REQUIRE(pkbManager.isPrintStmt(stmt[1]));
        REQUIRE(pkbManager.getPrints() == FILLED_SET_2);
    }

    SECTION("add calls") {
        REQUIRE(pkbManager.getCalls() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isCallStmt(stmt[0]));
        REQUIRE_NOTHROW(pkbManager.registerCall(stmt[0]));
        REQUIRE(pkbManager.isCallStmt(stmt[0]));
        REQUIRE(pkbManager.getCalls() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isCallStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerCall(stmt[1]));
        REQUIRE(pkbManager.isCallStmt(stmt[1]));
        REQUIRE(pkbManager.getCalls() == FILLED_SET_2);
    }

    SECTION("prevents duplicate") {
        REQUIRE_NOTHROW(pkbManager.registerRead(stmt[0]));

        REQUIRE_THROWS(pkbManager.registerAssign(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerPrint(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerIf(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerCall(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerWhile(stmt[0]));

        REQUIRE_NOTHROW(pkbManager.registerRead(stmt[0]));
    }

    SECTION("correct count") {
        REQUIRE(pkbManager.getStatementCount() == 0);

        pkbManager.registerRead(stmt[0]);
        REQUIRE(pkbManager.getStatementCount() == 1);

        pkbManager.registerIf(stmt[1]);
        REQUIRE(pkbManager.getStatementCount() == 2);

        pkbManager.registerVariable("abc");
        REQUIRE(pkbManager.getStatementCount() == 2);

        pkbManager.registerAssign(stmt[2]);
        pkbManager.registerAssign(stmt[2]);
        REQUIRE(pkbManager.getStatementCount() == 3);

        pkbManager.registerPrint(stmt[3]);
        REQUIRE(pkbManager.getStatementCount() == 4);
    }
}

TEST_CASE("PKB: uses abstraction") {
    unordered_set<string> varList;
    vector<pair<string, string>> entryList;
    string var[] = {"v1", "v2"};

    PKB pkbManager;

    SECTION("UsesS") {
        unordered_set<string> stmtList;
        string stmt[] = {"1", "2"};

        REQUIRE(sortAndCompareVectors(pkbManager.getAllUsesS(), entryList));
        REQUIRE(pkbManager.getUsesByStmt(stmt[0]) == varList);
        REQUIRE(pkbManager.getUsesSByVar(var[0]) == stmtList);
        REQUIRE_FALSE(pkbManager.isUsesS(stmt[0], var[0]));

        pkbManager.registerUsesS(stmt[0], var[0]);
        entryList.push_back(make_pair(stmt[0], var[0]));
        varList.insert(var[0]);
        stmtList.insert(stmt[0]);

        REQUIRE(pkbManager.isUsesS(stmt[0], var[0]));
        REQUIRE(pkbManager.getUsesByStmt(stmt[0]) == varList);
        REQUIRE(pkbManager.getUsesSByVar(var[0]) == stmtList);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllUsesS(), entryList));

        REQUIRE(pkbManager.getAllStmtsUsingSomeVar() == unordered_set<string>({stmt[0]}));
    }

    SECTION("UsesP") {
        unordered_set<string> procList;
        string proc[] = {"p1", "p2"};

        REQUIRE(sortAndCompareVectors(pkbManager.getAllUsesP(), entryList));
        REQUIRE(pkbManager.getUsesPByVar(var[0]) == procList);
        REQUIRE(pkbManager.getUsesByProc(proc[0]) == varList);
        REQUIRE_FALSE(pkbManager.isUsesP(proc[0], var[0]));

        pkbManager.registerUsesP(proc[0], var[0]);
        entryList.push_back(make_pair(proc[0], var[0]));
        varList.insert(var[0]);
        procList.insert(proc[0]);

        REQUIRE(pkbManager.isUsesP(proc[0], var[0]));
        REQUIRE(pkbManager.getUsesByProc(proc[0]) == varList);
        REQUIRE(pkbManager.getUsesPByVar(var[0]) == procList);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllUsesP(), entryList));

        REQUIRE(pkbManager.getAllProcsUsingSomeVar() == unordered_set<string>({proc[0]}));
    }
}

TEST_CASE("PKB: modifies abstraction") {
    unordered_set<string> varList;
    vector<pair<string, string>> entryList;
    string var[] = {"v1", "v2"};

    PKB pkbManager;

    SECTION("ModifiesS") {
        unordered_set<string> stmtList;
        string stmt[] = {"1", "2"};

        REQUIRE(sortAndCompareVectors(pkbManager.getAllModifiesS(), entryList));
        REQUIRE(pkbManager.getModifiesByStmt(stmt[0]) == varList);
        REQUIRE(pkbManager.getModifiesSByVar(var[0]) == stmtList);
        REQUIRE_FALSE(pkbManager.isModifiesS(stmt[0], var[0]));

        pkbManager.registerModifiesS(stmt[0], var[0]);
        entryList.push_back(make_pair(stmt[0], var[0]));
        varList.insert(var[0]);
        stmtList.insert(stmt[0]);

        REQUIRE(pkbManager.isModifiesS(stmt[0], var[0]));
        REQUIRE(pkbManager.getModifiesByStmt(stmt[0]) == varList);
        REQUIRE(pkbManager.getModifiesSByVar(var[0]) == stmtList);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllModifiesS(), entryList));

        REQUIRE(pkbManager.getAllStmtsModifyingSomeVar() == unordered_set<string>({stmt[0]}));
    }

    SECTION("ModifiesP") {
        unordered_set<string> procList;
        string proc[] = {"p1", "p2"};

        REQUIRE(sortAndCompareVectors(pkbManager.getAllModifiesP(), entryList));
        REQUIRE(pkbManager.getModifiesPByVar(var[0]) == procList);
        REQUIRE(pkbManager.getModifiesByProc(proc[0]) == varList);
        REQUIRE_FALSE(pkbManager.isModifiesP(proc[0], var[0]));

        pkbManager.registerModifiesP(proc[0], var[0]);
        entryList.push_back(make_pair(proc[0], var[0]));
        varList.insert(var[0]);
        procList.insert(proc[0]);

        REQUIRE(pkbManager.isModifiesP(proc[0], var[0]));
        REQUIRE(pkbManager.getModifiesByProc(proc[0]) == varList);
        REQUIRE(pkbManager.getModifiesPByVar(var[0]) == procList);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllModifiesP(), entryList));

        REQUIRE(pkbManager.getAllProcsModifyingSomeVar() == unordered_set<string>({proc[0]}));
    }
}

TEST_CASE("PKB: follows abstraction") {
    unordered_set<string> EMPTY_LIST;
    vector<pair<string, string>> entryList;
    string stmt[] = {"1", "2", "3"};

    PKB pkbManager;

    SECTION("Follows") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllFollows(), entryList));
        REQUIRE(pkbManager.getStmtFollowing(stmt[0]) == "");
        REQUIRE(pkbManager.getStmtFollowedBy(stmt[0]) == "");
        REQUIRE_FALSE(pkbManager.isFollows(stmt[0], stmt[1]));

        pkbManager.registerFollows(stmt[0], stmt[1]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));

        REQUIRE(pkbManager.isFollows(stmt[0], stmt[1]));
        REQUIRE(pkbManager.getStmtFollowedBy(stmt[0]) == stmt[1]);
        REQUIRE(pkbManager.getStmtFollowing(stmt[1]) == stmt[0]);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllFollows(), entryList));
    }

    SECTION("FollowsT") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllFollowsT(), entryList));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == EMPTY_LIST);
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == EMPTY_LIST);
        REQUIRE_FALSE(pkbManager.isFollowsT(stmt[0], stmt[1]));

        // 0 -> 1
        // 1 -> 2
        pkbManager.registerFollows(stmt[0], stmt[1]);
        pkbManager.registerFollows(stmt[1], stmt[2]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));
        entryList.push_back(make_pair(stmt[0], stmt[2]));
        entryList.push_back(make_pair(stmt[1], stmt[2]));

        REQUIRE(pkbManager.isFollowsT(stmt[0], stmt[1]));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == unordered_set<string>({stmt[1], stmt[2]}));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[1]) == unordered_set<string>({stmt[2]}));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[2]) == unordered_set<string>());
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[0]) == unordered_set<string>());
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[1]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[2]) == unordered_set<string>({stmt[0], stmt[1]}));
        REQUIRE(pkbManager.isFollowsT(stmt[0], stmt[2]));
    }
}

TEST_CASE("PKB: parent abstraction") {
    unordered_set<string> EMPTY_LIST;
    vector<pair<string, string>> entryList;
    string stmt[] = {"1", "2", "3", "4"};

    PKB pkbManager;

    SECTION("Parent") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllParent(), entryList));
        REQUIRE(pkbManager.getChildStmtsOf(stmt[0]) == EMPTY_LIST);
        REQUIRE(pkbManager.getParentOf(stmt[0]) == "");
        REQUIRE_FALSE(pkbManager.isParent(stmt[0], stmt[1]));

        // 0 -> {1, 2}
        pkbManager.registerParent(stmt[0], stmt[1]);
        pkbManager.registerParent(stmt[0], stmt[2]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));
        entryList.push_back(make_pair(stmt[0], stmt[2]));

        REQUIRE(pkbManager.isParent(stmt[0], stmt[1]));
        REQUIRE(pkbManager.getChildStmtsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2]}));
        REQUIRE(pkbManager.getParentOf(stmt[1]) == stmt[0]);
        REQUIRE(pkbManager.getParentOf(stmt[2]) == stmt[0]);
        REQUIRE(sortAndCompareVectors(pkbManager.getAllParent(), entryList));
    }

    SECTION("ParentT") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllParentT(), entryList));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[0]) == EMPTY_LIST);
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[0]) == EMPTY_LIST);
        REQUIRE_FALSE(pkbManager.isParentT(stmt[0], stmt[1]));

        // 0 -> {1, 2}
        // 1 -> {3}
        pkbManager.registerParent(stmt[0], stmt[1]);
        pkbManager.registerParent(stmt[0], stmt[2]);
        pkbManager.registerParent(stmt[1], stmt[3]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));
        entryList.push_back(make_pair(stmt[0], stmt[2]));
        entryList.push_back(make_pair(stmt[0], stmt[3]));
        entryList.push_back(make_pair(stmt[1], stmt[3]));

        REQUIRE(sortAndCompareVectors(pkbManager.getAllParentT(), entryList));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[3]) == unordered_set<string>({stmt[0], stmt[1]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[2]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[1]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[0]) == unordered_set<string>());

        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2], stmt[3]}));
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[1]) == unordered_set<string>({stmt[3]}));
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[2]) == unordered_set<string>());

        REQUIRE(pkbManager.isParentT(stmt[0], stmt[3]));
        REQUIRE_FALSE(pkbManager.isParentT(stmt[2], stmt[3]));
    }
}
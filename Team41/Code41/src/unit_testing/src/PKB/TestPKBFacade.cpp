#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PKB: entities abstraction") {
    unordered_set<string> EMPTY_SET;
    string entity[] = {"abc", "def"};
    unordered_set<string> FILLED_SET_1;
    FILLED_SET_1.insert(entity[0]);
    unordered_set<string> FILLED_SET_2;
    FILLED_SET_2.insert(entity[0]);
    FILLED_SET_2.insert(entity[1]);

    PKBManager pkbManager;

    SECTION("add variables") {
        REQUIRE(pkbManager.getVariables() == EMPTY_SET);
        REQUIRE(pkbManager.getVariableCount() == 0);
        REQUIRE_NOTHROW(pkbManager.registerVariable(entity[0]));
        REQUIRE(pkbManager.getVariables() == FILLED_SET_1);
        REQUIRE(pkbManager.getVariableCount() == 1);
        REQUIRE_NOTHROW(pkbManager.registerVariable(entity[1]));
        REQUIRE(pkbManager.getVariables() == FILLED_SET_2);
        REQUIRE(pkbManager.getVariableCount() == 2);
        // check that other table are not affected
        REQUIRE(pkbManager.getConstants() == EMPTY_SET);
    }

    SECTION("add constants") {
        REQUIRE(pkbManager.getConstants() == EMPTY_SET);
        REQUIRE(pkbManager.getConstantCount() == 0);
        REQUIRE_NOTHROW(pkbManager.registerConstant(entity[0]));
        REQUIRE(pkbManager.getConstants() == FILLED_SET_1);
        REQUIRE(pkbManager.getConstantCount() == 1);
        REQUIRE_NOTHROW(pkbManager.registerConstant(entity[1]));
        REQUIRE(pkbManager.getConstants() == FILLED_SET_2);
        REQUIRE(pkbManager.getConstantCount() == 2);
    }

    SECTION("add procedure") {
        REQUIRE(pkbManager.getProcedures() == EMPTY_SET);
        REQUIRE(pkbManager.getProcedureCount() == 0);
        REQUIRE_NOTHROW(pkbManager.registerProcedure(entity[0]));
        REQUIRE(pkbManager.getProcedures() == FILLED_SET_1);
        REQUIRE(pkbManager.getProcedureCount() == 1);
        REQUIRE_NOTHROW(pkbManager.registerProcedure(entity[1]));
        REQUIRE(pkbManager.getProcedures() == FILLED_SET_2);
        REQUIRE(pkbManager.getProcedureCount() == 2);
    }
}

TEST_CASE("PKB: statements abstraction") {
    unordered_set<string> EMPTY_SET;
    string stmt[] = {"1", "2", "3", "4"};
    unordered_set<string> FILLED_SET_1;
    FILLED_SET_1.insert(stmt[0]);
    unordered_set<string> FILLED_SET_2;
    FILLED_SET_2.insert(stmt[0]);
    FILLED_SET_2.insert(stmt[1]);

    PKBManager pkbManager;

    SECTION("add assign") {
        REQUIRE(pkbManager.getAssigns() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isAssignStmt(stmt[0]));
        REQUIRE(pkbManager.getAssignCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerAssignStmt(stmt[0]));
        REQUIRE(pkbManager.isAssignStmt(stmt[0]));
        REQUIRE(pkbManager.getAssigns() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isAssignStmt(stmt[1]));
        REQUIRE(pkbManager.getAssignCount() == 1);

        REQUIRE_NOTHROW(pkbManager.registerAssignStmt(stmt[1]));
        REQUIRE(pkbManager.isAssignStmt(stmt[1]));
        REQUIRE(pkbManager.getAssigns() == FILLED_SET_2);
        REQUIRE(pkbManager.getAssignCount() == 2);

        // check that other table are not affected
        REQUIRE(pkbManager.getCalls() == EMPTY_SET);
    }

    SECTION("add while") {
        REQUIRE(pkbManager.getWhiles() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isWhileStmt(stmt[0]));
        REQUIRE(pkbManager.getWhileCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerWhileStmt(stmt[0]));
        REQUIRE(pkbManager.isWhileStmt(stmt[0]));
        REQUIRE(pkbManager.getWhiles() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isWhileStmt(stmt[1]));
        REQUIRE(pkbManager.getWhileCount() == 1);

        REQUIRE_NOTHROW(pkbManager.registerWhileStmt(stmt[1]));
        REQUIRE(pkbManager.isWhileStmt(stmt[1]));
        REQUIRE(pkbManager.getWhiles() == FILLED_SET_2);
        REQUIRE(pkbManager.getWhileCount() == 2);
    }

    SECTION("add ifs") {
        REQUIRE(pkbManager.getIfs() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isIfStmt(stmt[0]));
        REQUIRE(pkbManager.getIfCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerIfStmt(stmt[0]));
        REQUIRE(pkbManager.isIfStmt(stmt[0]));
        REQUIRE(pkbManager.getIfs() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isIfStmt(stmt[1]));
        REQUIRE(pkbManager.getIfCount() == 1);

        REQUIRE_NOTHROW(pkbManager.registerIfStmt(stmt[1]));
        REQUIRE(pkbManager.isIfStmt(stmt[1]));
        REQUIRE(pkbManager.getIfs() == FILLED_SET_2);
        REQUIRE(pkbManager.getIfCount() == 2);
    }

    SECTION("add reads") {
        REQUIRE(pkbManager.getReads() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isReadStmt(stmt[0]));
        REQUIRE(pkbManager.getReadCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerReadStmt(stmt[0]));
        REQUIRE(pkbManager.isReadStmt(stmt[0]));
        REQUIRE(pkbManager.getReads() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isReadStmt(stmt[1]));
        REQUIRE(pkbManager.getReadCount() == 1);

        REQUIRE_NOTHROW(pkbManager.registerReadStmt(stmt[1]));
        REQUIRE(pkbManager.isReadStmt(stmt[1]));
        REQUIRE(pkbManager.getReads() == FILLED_SET_2);
        REQUIRE(pkbManager.getReadCount() == 2);
    }

    SECTION("add prints") {
        REQUIRE(pkbManager.getPrints() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isPrintStmt(stmt[0]));
        REQUIRE(pkbManager.getPrintCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerPrintStmt(stmt[0]));
        REQUIRE(pkbManager.isPrintStmt(stmt[0]));
        REQUIRE(pkbManager.getPrints() == FILLED_SET_1);
        REQUIRE(pkbManager.getPrintCount() == 1);

        REQUIRE_FALSE(pkbManager.isPrintStmt(stmt[1]));
        REQUIRE_NOTHROW(pkbManager.registerPrintStmt(stmt[1]));
        REQUIRE(pkbManager.isPrintStmt(stmt[1]));
        REQUIRE(pkbManager.getPrints() == FILLED_SET_2);
        REQUIRE(pkbManager.getPrintCount() == 2);
    }

    SECTION("add calls") {
        REQUIRE(pkbManager.getCalls() == EMPTY_SET);
        REQUIRE_FALSE(pkbManager.isCallStmt(stmt[0]));
        REQUIRE(pkbManager.getCallCount() == 0);

        REQUIRE_NOTHROW(pkbManager.registerCallStmt(stmt[0]));
        REQUIRE(pkbManager.isCallStmt(stmt[0]));
        REQUIRE(pkbManager.getCalls() == FILLED_SET_1);
        REQUIRE_FALSE(pkbManager.isCallStmt(stmt[1]));
        REQUIRE(pkbManager.getCallCount() == 1);

        REQUIRE_NOTHROW(pkbManager.registerCallStmt(stmt[1]));
        REQUIRE(pkbManager.isCallStmt(stmt[1]));
        REQUIRE(pkbManager.getCalls() == FILLED_SET_2);
        REQUIRE(pkbManager.getCallCount() == 2);
    }

    SECTION("prevents duplicate") {
        REQUIRE_NOTHROW(pkbManager.registerReadStmt(stmt[0]));

        REQUIRE_THROWS(pkbManager.registerAssignStmt(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerPrintStmt(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerIfStmt(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerCallStmt(stmt[0]));
        REQUIRE_THROWS(pkbManager.registerWhileStmt(stmt[0]));

        REQUIRE_NOTHROW(pkbManager.registerReadStmt(stmt[0]));
    }

    SECTION("correct count") {
        REQUIRE(pkbManager.getStatementCount() == 0);

        pkbManager.registerReadStmt(stmt[0]);
        REQUIRE(pkbManager.getStatementCount() == 1);

        pkbManager.registerIfStmt(stmt[1]);
        REQUIRE(pkbManager.getStatementCount() == 2);

        pkbManager.registerVariable("abc");
        REQUIRE(pkbManager.getStatementCount() == 2);

        pkbManager.registerAssignStmt(stmt[2]);
        pkbManager.registerAssignStmt(stmt[2]);
        REQUIRE(pkbManager.getStatementCount() == 3);

        pkbManager.registerPrintStmt(stmt[3]);
        REQUIRE(pkbManager.getStatementCount() == 4);
    }
}

TEST_CASE("PKB: uses abstraction") {
    unordered_set<string> varList;
    vector<pair<string, string>> entryList;
    string var[] = {"v1", "v2"};

    PKBManager pkbManager;

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

    PKBManager pkbManager;

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

    PKBManager pkbManager;

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

        REQUIRE(pkbManager.getAllStmtsFollowingSomeStmt() == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAllStmtsFollowedBySomeStmt() == unordered_set<string>({stmt[1]}));
    }

    SECTION("FollowsT") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllFollowsT(), entryList));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == EMPTY_LIST);
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == EMPTY_LIST);
        REQUIRE_FALSE(pkbManager.isFollowsT(stmt[0], stmt[1]));

        // 0 -> 1
        // 1 -> 2
        pkbManager.registerFollowsT(stmt[0], stmt[1]);
        pkbManager.registerFollowsT(stmt[1], stmt[2]);
        pkbManager.registerFollowsT(stmt[0], stmt[2]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));
        entryList.push_back(make_pair(stmt[0], stmt[2]));
        entryList.push_back(make_pair(stmt[1], stmt[2]));

        REQUIRE(pkbManager.isFollowsT(stmt[0], stmt[1]));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[0]) == unordered_set<string>({stmt[1], stmt[2]}));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[1]) == unordered_set<string>({stmt[2]}));
        REQUIRE(pkbManager.getAllStmtsFollowedTBy(stmt[2]).empty());
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[0]).empty());
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[1]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAllStmtsFollowingT(stmt[2]) == unordered_set<string>({stmt[0], stmt[1]}));
        REQUIRE(pkbManager.isFollowsT(stmt[0], stmt[2]));
    }
}

TEST_CASE("PKB: parent abstraction") {
    unordered_set<string> EMPTY_LIST;
    vector<pair<string, string>> entryList;
    string stmt[] = {"1", "2", "3", "4"};

    PKBManager pkbManager;

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

        REQUIRE(pkbManager.getAllStmtsParentOfSomeStmt() == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAllStmtsChildOfSomeStmt() == unordered_set<string>({stmt[1], stmt[2]}));
    }

    SECTION("ParentT") {
        REQUIRE(sortAndCompareVectors(pkbManager.getAllParentT(), entryList));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[0]) == EMPTY_LIST);
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[0]) == EMPTY_LIST);
        REQUIRE_FALSE(pkbManager.isParentT(stmt[0], stmt[1]));

        // 0 -> {1, 2}
        // 1 -> {3}
        pkbManager.registerParentT(stmt[0], stmt[1]);
        pkbManager.registerParentT(stmt[0], stmt[2]);
        pkbManager.registerParentT(stmt[1], stmt[3]);
        pkbManager.registerParentT(stmt[0], stmt[3]);
        entryList.push_back(make_pair(stmt[0], stmt[1]));
        entryList.push_back(make_pair(stmt[0], stmt[2]));
        entryList.push_back(make_pair(stmt[0], stmt[3]));
        entryList.push_back(make_pair(stmt[1], stmt[3]));

        REQUIRE(sortAndCompareVectors(pkbManager.getAllParentT(), entryList));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[3]) == unordered_set<string>({stmt[0], stmt[1]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[2]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[1]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getAncestorStmtsOf(stmt[0]).empty());

        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[0]) == unordered_set<string>({stmt[1], stmt[2], stmt[3]}));
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[1]) == unordered_set<string>({stmt[3]}));
        REQUIRE(pkbManager.getDescendantStmtsOf(stmt[2]).empty());

        REQUIRE(pkbManager.isParentT(stmt[0], stmt[3]));
        REQUIRE_FALSE(pkbManager.isParentT(stmt[2], stmt[3]));
    }
}

TEST_CASE("PKB: calls abstraction") {
    vector<pair<string, string>> entryList;
    string proc[] = {"p0", "p1", "p2", "p3"};

    PKBManager pkbManager;

    SECTION("Calls") {
        REQUIRE(pkbManager.getAllCalls().empty());
        REQUIRE(pkbManager.getAllProcCalledBy(proc[0]).empty());
        REQUIRE(pkbManager.getAllProcCalling(proc[0]).empty());
        REQUIRE_FALSE(pkbManager.isCalls(proc[0], proc[1]));

        // 0 -> {1, 2}
        pkbManager.registerCalls(proc[0], proc[1]);
        pkbManager.registerCalls(proc[0], proc[2]);
        entryList.push_back(make_pair(proc[0], proc[1]));
        entryList.push_back(make_pair(proc[0], proc[2]));

        REQUIRE(pkbManager.isCalls(proc[0], proc[1]));
        REQUIRE(pkbManager.getAllProcCalledBy(proc[0]) == unordered_set<string>({proc[1], proc[2]}));
        REQUIRE(pkbManager.getAllProcCalling(proc[1]) == unordered_set<string>({proc[0]}));
        REQUIRE(pkbManager.getAllProcCalling(proc[2]) == unordered_set<string>({proc[0]}));
        REQUIRE(sortAndCompareVectors(pkbManager.getAllCalls(), entryList));

        REQUIRE(pkbManager.getAllProcsCallingSomeProcs() == unordered_set<string>({proc[0]}));
        REQUIRE(pkbManager.getAllProcsCalledBySomeProcs() == unordered_set<string>({proc[1], proc[2]}));
    }

    SECTION("CallsT") {
        REQUIRE(pkbManager.getAllCallsT().empty());
        REQUIRE(pkbManager.getAllProcCalledTBy(proc[0]).empty());
        REQUIRE(pkbManager.getAllProcCallingT(proc[0]).empty());
        REQUIRE_FALSE(pkbManager.isCallsT(proc[0], proc[1]));

        // 0 -> {1, 2}
        // 1 -> {2, 3}
        pkbManager.registerCallsT(proc[0], proc[1]);
        pkbManager.registerCallsT(proc[0], proc[2]);
        pkbManager.registerCallsT(proc[1], proc[2]);
        pkbManager.registerCallsT(proc[1], proc[3]);
        pkbManager.registerCallsT(proc[0], proc[3]);
        entryList.push_back(make_pair(proc[0], proc[1]));
        entryList.push_back(make_pair(proc[0], proc[2]));
        entryList.push_back(make_pair(proc[0], proc[3]));
        entryList.push_back(make_pair(proc[1], proc[2]));
        entryList.push_back(make_pair(proc[1], proc[3]));

        REQUIRE(sortAndCompareVectors(pkbManager.getAllCallsT(), entryList));
        REQUIRE(pkbManager.getAllProcCallingT(proc[3]) == unordered_set<string>({proc[0], proc[1]}));
        REQUIRE(pkbManager.getAllProcCallingT(proc[2]) == unordered_set<string>({proc[0], proc[1]}));
        REQUIRE(pkbManager.getAllProcCallingT(proc[1]) == unordered_set<string>({proc[0]}));
        REQUIRE(pkbManager.getAllProcCallingT(proc[0]).empty());

        REQUIRE(pkbManager.getAllProcCalledTBy(proc[0]) == unordered_set<string>({proc[1], proc[2], proc[3]}));
        REQUIRE(pkbManager.getAllProcCalledTBy(proc[1]) == unordered_set<string>({proc[2], proc[3]}));
        REQUIRE(pkbManager.getAllProcCalledTBy(proc[2]).empty());
        REQUIRE(pkbManager.getAllProcCalledTBy(proc[3]).empty());

        REQUIRE(pkbManager.isCallsT(proc[0], proc[3]));
        REQUIRE_FALSE(pkbManager.isCallsT(proc[2], proc[3]));
    }
}

TEST_CASE("PKB: pattern abstraction") {
    PKBManager pkbManager;
    string stmt[] = {"s0", "s1", "s2"};
    string vars[] = {"v0", "v1", "v2"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    SECTION("Assign") {
        Token *varTok = Token::makeName("v0");
        Token *oneTok = Token::makeConst("1");
        Token *twoTok = Token::makeConst("2");

        // 1 + 2 * v0
        TNode *varNode = TNode::makeVarName(varTok);
        TNode *one = TNode::makeConstVal(oneTok);
        TNode *two = TNode::makeConstVal(twoTok);
        TNode *times = TNode::makeTimes(two, varNode);
        TNode *plus = TNode::makePlus(one, times);

        SECTION("Pattern") {
            REQUIRE(sortAndCompareVectors(pkbManager.getAssignStmtNVarFromPattern(plus), EMPTY_SET_PAIR));
            REQUIRE(pkbManager.getAssignStmtFromPattern(plus).empty());
            REQUIRE(pkbManager.getAssignStmtFromPatternNVar(plus, vars[0]).empty());

            // v0 = 1 + 2 * v0
            REQUIRE_NOTHROW(pkbManager.registerAssignPattern(stmt[0], vars[0], plus));
            REQUIRE(sortAndCompareVectors(pkbManager.getAssignStmtNVarFromPattern(plus),
                                          vector<pair<string, string>>({{stmt[0], vars[0]}})));
            REQUIRE(pkbManager.getAssignStmtFromPattern(plus) == unordered_set<string>({stmt[0]}));
            REQUIRE(pkbManager.getAssignStmtFromPatternNVar(plus, vars[0]) == unordered_set<string>({stmt[0]}));
            REQUIRE(pkbManager.getAssignStmtFromPatternNVar(plus, vars[1]).empty());
        }

        SECTION("SubPattern") {
            REQUIRE(sortAndCompareVectors(pkbManager.getAssignStmtNVarFromSubpattern(plus), EMPTY_SET_PAIR));
            REQUIRE(pkbManager.getAssignStmtFromSubpattern(plus).empty());
            REQUIRE(pkbManager.getAssignStmtFromSubpatternNVar(plus, vars[0]).empty());
            // v0 = 1 + 2 * v0
            REQUIRE_NOTHROW(pkbManager.registerAssignPattern(stmt[0], vars[0], plus));

            for (TNode *child: {one, two, times, plus, varNode}) {
                REQUIRE(sortAndCompareVectors(pkbManager.getAssignStmtNVarFromSubpattern(child),
                                              vector<pair<string, string>>({{stmt[0], vars[0]}})));
                REQUIRE(pkbManager.getAssignStmtFromSubpattern(child) == unordered_set<string>({stmt[0]}));
                REQUIRE(pkbManager.getAssignStmtFromSubpatternNVar(child, vars[0]) == unordered_set<string>({stmt[0]}));
                REQUIRE(pkbManager.getAssignStmtFromSubpatternNVar(child, vars[1]).empty());
            }
        }

        delete plus;
    }

    SECTION("If") {
        REQUIRE(pkbManager.getIfStmtNVarPairs().empty());
        REQUIRE(pkbManager.getIfStmtUsingSomeVarCond().empty());
        REQUIRE(pkbManager.getIfStmtUsingVarCond(vars[0]).empty());

        REQUIRE_NOTHROW(pkbManager.registerIfPattern(stmt[0], vars[0]));
        REQUIRE(sortAndCompareVectors(pkbManager.getIfStmtNVarPairs(),
                                      vector<pair<string, string>>({{stmt[0], vars[0]}})));
        REQUIRE(pkbManager.getIfStmtUsingSomeVarCond() == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getIfStmtUsingVarCond(vars[0]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getIfStmtUsingVarCond(vars[1]).empty());
    }

    SECTION("While") {
        REQUIRE(pkbManager.getWhileStmtNVarPairs().empty());
        REQUIRE(pkbManager.getWhileStmtUsingSomeVarCond().empty());
        REQUIRE(pkbManager.getWhileStmtUsingVarCond(vars[0]).empty());

        REQUIRE_NOTHROW(pkbManager.registerWhilePattern(stmt[0], vars[0]));
        REQUIRE(sortAndCompareVectors(pkbManager.getWhileStmtNVarPairs(),
                                      vector<pair<string, string>>({{stmt[0], vars[0]}})));
        REQUIRE(pkbManager.getWhileStmtUsingSomeVarCond() == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getWhileStmtUsingVarCond(vars[0]) == unordered_set<string>({stmt[0]}));
        REQUIRE(pkbManager.getWhileStmtUsingVarCond(vars[1]).empty());
    }
}
#include "catch.hpp"
#include "Common/CFGBuilder.h"
#include "PKB/PKBManager.h"
#include "QPS/Evaluator/NextEvaluator.h"
#include "QPS/Evaluator/NextTEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "./TableTestUtils.h"

TEST_CASE("Evaluator: Next and NextT evaluator") {
    string stmt[] = {"1", "2", "3", "4", "5"};
    PKBManager *pkbManager = new PKBManager();
    NextKBAdapter *nextKbAdapter = new NextKBAdapter(pkbManager);
    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2", "5"}},
            {"2", {"3"}},
            {"3", {"1", "4"}},
            {"4", {"5"}},
            {"5", {}},
    };

    string startStmt = "1";
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {startStmt});
    cfgBuilder.build();
    CFGNode *rootNode = cfgBuilder.getCFG();
    unordered_map<string, CFGNode *> stmtToCFG = cfgBuilder.getStmtNumToNodeMap();

    pkbManager->registerCFG(rootNode, stmtToCFG);
    pkbManager->registerAssignStmt(stmt[0]);
    pkbManager->registerReadStmt(stmt[1], "v");
    pkbManager->registerAssignStmt(stmt[2]);
    pkbManager->registerPrintStmt(stmt[3], "v");
    pkbManager->registerAssignStmt(stmt[4]);

    ClauseVariable integer1(ClauseVariable::variable_type::integer, "1", new ConstantEntities());
    ClauseVariable integer2(ClauseVariable::variable_type::integer, "2", new ConstantEntities());
    ClauseVariable integer3(ClauseVariable::variable_type::integer, "3", new ConstantEntities());
    ClauseVariable integer4(ClauseVariable::variable_type::integer, "4", new ConstantEntities());
    ClauseVariable integer5(ClauseVariable::variable_type::integer, "5", new ConstantEntities());

    ClauseVariable synonymAssign1(ClauseVariable::variable_type::synonym, "a1", new AssignEntities());
    ClauseVariable synonymAssign2(ClauseVariable::variable_type::synonym, "a2", new AssignEntities());
    ClauseVariable synonymStmt1(ClauseVariable::variable_type::synonym, "s1", new StmtEntities());
    ClauseVariable synonymStmt2(ClauseVariable::variable_type::synonym, "s2", new StmtEntities());
    ClauseVariable synonymRead(ClauseVariable::variable_type::synonym, "r1", new ReadEntities());

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", new NoneEntities());

    SECTION("Next Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::next, integer1, integer5);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::next, integer3, integer1);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::next, integer1, integer2);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::next, integer1, integer3);
            Table *table4 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(table4->isFalseTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::next, integer1, synonymStmt1);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"2"}, {"5"}}));

            QueryClause queryClause2(QueryClause::next, integer2, synonymStmt1);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymStmt1.getLabel()}, {{"3"}}));

            QueryClause queryClause3(QueryClause::next, integer3, synonymStmt1);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"4"}}));

            QueryClause queryClause4(QueryClause::next, integer4, synonymStmt1);
            Table *table4 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {synonymStmt1.getLabel()}, {{"5"}}));

            QueryClause queryClause5(QueryClause::next, integer5, synonymStmt1);
            Table *table5 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(table5->isFalseTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::next, integer1, wildcard);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::next, integer5, wildcard);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isFalseTable());

            QueryClause queryClause3(QueryClause::next, integer3, wildcard);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::next, synonymStmt1, integer1);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"3"}}));

            QueryClause queryClause2(QueryClause::next, synonymAssign1, integer2);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel()}, {{"1"}}));

            QueryClause queryClause3(QueryClause::next, synonymRead, integer3);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymRead.getLabel()}, {{"2"}}));

            QueryClause queryClause4(QueryClause::next, synonymStmt1, integer4);
            Table *table4 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {synonymStmt1.getLabel()}, {{"3"}}));

            QueryClause queryClause5(QueryClause::next, synonymStmt1, integer5);
            Table *table5 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(TableTestUtils::checkTableMatches(table5, {synonymStmt1.getLabel()}, {{"4"},
                                                                                          {"1"}}));
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::next, synonymStmt1, synonymStmt2);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel(), synonymStmt2.getLabel()},
                                                      {{"1", "2"},
                                                       {"2", "3"},
                                                       {"3", "4"},
                                                       {"4", "5"},
                                                       {"3", "1"},
                                                       {"1", "5"}}));

            QueryClause queryClause2(QueryClause::next, synonymAssign1, synonymAssign2);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel(), synonymAssign2.getLabel()},
                                                      {{"3", "1"},
                                                       {"1", "5"}}));

            delete table1;
            delete table2;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::next, synonymStmt1, wildcard);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"}}));

            QueryClause queryClause2(QueryClause::next, synonymAssign1, wildcard);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel()}, {{"1"},
                                                                                            {"3"}}));

            QueryClause queryClause3(QueryClause::next, synonymRead, wildcard);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymRead.getLabel()}, {{"2"}}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::next, wildcard, integer1);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::next, wildcard, integer2);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::next, wildcard, integer4);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::next, wildcard, integer5);
            Table *table4 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            QueryClause queryClause5(QueryClause::next, wildcard, integer5);
            Table *table5 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(table5->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::next, wildcard, synonymStmt1);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"},
                                                                                          {"5"}}));

            QueryClause queryClause2(QueryClause::next, wildcard, synonymAssign1);
            Table *table2 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel()}, {{"1"},
                                                                                            {"3"},
                                                                                            {"5"}}));

            QueryClause queryClause3(QueryClause::next, wildcard, synonymRead);
            Table *table3 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymRead.getLabel()}, {{"2"}}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::next, wildcard, wildcard);
            Table *table1 = NextEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());
            delete table1;
        }
    }

    SECTION("NextT Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::nextT, integer1, integer5);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::nextT, integer3, integer1);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::nextT, integer1, integer2);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::nextT, integer1, integer3);
            Table *table4 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            QueryClause queryClause5(QueryClause::nextT, integer5, integer1);
            Table *table5 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(table5->isFalseTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause5(QueryClause::nextT, integer5, synonymStmt1);
            Table *table5 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(table5->isFalseTable());

            QueryClause queryClause4(QueryClause::nextT, integer4, synonymStmt1);
            Table *table4 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {synonymStmt1.getLabel()}, {{"5"}}));

            QueryClause queryClause3(QueryClause::nextT, integer3, synonymStmt1);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"},
                                                                                          {"5"}}));

            QueryClause queryClause2(QueryClause::nextT, integer2, synonymStmt1);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"},
                                                                                          {"5"}}));

            QueryClause queryClause1(QueryClause::nextT, integer1, synonymStmt1);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"},
                                                                                          {"5"}}));

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::nextT, integer1, wildcard);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::nextT, integer5, wildcard);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isFalseTable());

            QueryClause queryClause3(QueryClause::nextT, integer3, wildcard);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::nextT, synonymStmt1, integer1);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"}}));

            QueryClause queryClause2(QueryClause::nextT, synonymStmt1, integer2);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"}}));

            QueryClause queryClause3(QueryClause::nextT, synonymStmt1, integer3);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"}}));

            QueryClause queryClause4(QueryClause::nextT, synonymStmt1, integer4);
            Table *table4 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"}}));

            QueryClause queryClause5(QueryClause::nextT, synonymStmt1, integer5);
            Table *table5 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(TableTestUtils::checkTableMatches(table5, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"}}));

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::nextT, synonymStmt1, synonymStmt2);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel(), synonymStmt2.getLabel()},
                                                      {{"1", "1"},
                                                       {"1", "2"},
                                                       {"1", "3"},
                                                       {"1", "4"},
                                                       {"1", "5"},
                                                       {"2", "1"},
                                                       {"2", "2"},
                                                       {"2", "3"},
                                                       {"2", "4"},
                                                       {"2", "5"},
                                                       {"3", "1"},
                                                       {"3", "2"},
                                                       {"3", "3"},
                                                       {"3", "4"},
                                                       {"3", "5"},
                                                       {"4", "5"}}));

            QueryClause queryClause2(QueryClause::nextT, synonymAssign1, synonymAssign2);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel(), synonymAssign2.getLabel()},
                                                      {{"1", "1"},
                                                       {"1", "3"},
                                                       {"1", "5"},
                                                       {"3", "1"},
                                                       {"3", "3"},
                                                       {"3", "5"}}));

            delete table1;
            delete table2;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::nextT, synonymStmt1, wildcard);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"}}));

            QueryClause queryClause2(QueryClause::nextT, synonymAssign1, wildcard);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel()}, {{"1"},
                                                                                            {"3"}}));

            QueryClause queryClause3(QueryClause::nextT, synonymRead, wildcard);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymRead.getLabel()}, {{"2"}}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::nextT, wildcard, integer1);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::nextT, wildcard, integer2);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::nextT, wildcard, integer4);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::nextT, wildcard, integer5);
            Table *table4 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            QueryClause queryClause5(QueryClause::nextT, wildcard, integer5);
            Table *table5 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause5);
            REQUIRE(table5->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::nextT, wildcard, synonymStmt1);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {synonymStmt1.getLabel()}, {{"1"},
                                                                                          {"2"},
                                                                                          {"3"},
                                                                                          {"4"},
                                                                                          {"5"}}));

            QueryClause queryClause2(QueryClause::nextT, wildcard, synonymAssign1);
            Table *table2 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {synonymAssign1.getLabel()}, {{"1"},
                                                                                            {"3"},
                                                                                            {"5"}}));

            QueryClause queryClause3(QueryClause::nextT, wildcard, synonymRead);
            Table *table3 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {synonymRead.getLabel()}, {{"2"}}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::nextT, wildcard, wildcard);
            Table *table1 = NextTEvaluator(pkbManager, nextKbAdapter).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());
            delete table1;
        }
    }

    delete nextKbAdapter;
    delete pkbManager;
}
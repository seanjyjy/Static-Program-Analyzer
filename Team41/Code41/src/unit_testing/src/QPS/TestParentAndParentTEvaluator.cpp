#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/ParentTable.h"
#include "QPS/Evaluator/ParentEvaluator.h"
#include "QPS/Evaluator/ParentTEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: Parent and ParentT evaluator") {
    string stmt[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    PKBManager* pkbManager = new PKBManager();

    /*
        1 x = 1;
        2 if (x == 1) then {
        3    y = 1;
          } else {
        4    t = 1;
        5    z = 1;
        6    if (y == 1) then {
        7       j = 2;
             } else {
        8       j = 3;
             }
          }
        9 j = 1;
     */
    pkbManager->registerParent(stmt[1], stmt[2]); // 2 <- 3
    pkbManager->registerParent(stmt[1], stmt[3]); // 2 <- 4
    pkbManager->registerParent(stmt[1], stmt[4]); // 2 <- 5
    pkbManager->registerParent(stmt[1], stmt[5]); // 2 <- 6
    pkbManager->registerParent(stmt[5], stmt[6]); // 6 <- 7
    pkbManager->registerParent(stmt[5], stmt[7]); // 6 <- 8

    pkbManager->registerParentT(stmt[1], stmt[2]);
    pkbManager->registerParentT(stmt[1], stmt[3]);
    pkbManager->registerParentT(stmt[1], stmt[4]);
    pkbManager->registerParentT(stmt[1], stmt[5]);
    pkbManager->registerParentT(stmt[5], stmt[6]);
    pkbManager->registerParentT(stmt[5], stmt[7]);
    pkbManager->registerParentT(stmt[1], stmt[6]); // 2 <- 7
    pkbManager->registerParentT(stmt[1], stmt[7]); // 2 <- 8

    ClauseVariable integer1(ClauseVariable::variable_type::integer, "1", QueryDeclaration::CONSTANT);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, "2", QueryDeclaration::CONSTANT);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, "3", QueryDeclaration::CONSTANT);
    ClauseVariable integer4(ClauseVariable::variable_type::integer, "4", QueryDeclaration::CONSTANT);
    ClauseVariable integer5(ClauseVariable::variable_type::integer, "5", QueryDeclaration::CONSTANT);
    ClauseVariable integer6(ClauseVariable::variable_type::integer, "6", QueryDeclaration::CONSTANT);
    ClauseVariable integer7(ClauseVariable::variable_type::integer, "7", QueryDeclaration::CONSTANT);
    ClauseVariable integer8(ClauseVariable::variable_type::integer, "8", QueryDeclaration::CONSTANT);
    ClauseVariable integer9(ClauseVariable::variable_type::integer, "9", QueryDeclaration::CONSTANT);

    ClauseVariable synonymS1(ClauseVariable::variable_type::synonym, "s1", QueryDeclaration::ASSIGN);
    ClauseVariable synonymS2(ClauseVariable::variable_type::synonym, "s2", QueryDeclaration::READ);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    SECTION("Parent Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::parent, integer1, integer2);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parent, integer2, integer3);
            Table* table2 = ParentEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::parent, integer2, integer6);
            Table* table3 = ParentEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::parent, integer2, integer7);
            Table* table4 = ParentEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::FalseTable);

            QueryClause queryClause5(QueryClause::parent, integer2, integer8);
            Table* table5 = ParentEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::parent, integer1, synonymS1);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parent, integer2, synonymS1);
            Table* table2 = ParentEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 4);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"3", "4", "5", "6"}));

            QueryClause queryClause3(QueryClause::parent, integer5, synonymS1);
            Table* table3 = ParentEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::parent, integer6, synonymS1);
            Table* table4 = ParentEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->size() == 2);
            REQUIRE(table4->getColumn("s1") == unordered_set<string>({"7", "8"}));

            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::parent, integer1, wildcard);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parent, integer2, wildcard);
            Table* table2 = ParentEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::parent, integer4, wildcard);
            Table* table3 = ParentEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::parent, integer5, wildcard);
            Table* table4 = ParentEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::FalseTable);

            QueryClause queryClause5(QueryClause::parent, integer6, wildcard);
            Table* table5 = ParentEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::TrueTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::parent, synonymS1, integer1);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parent, synonymS1, integer2);
            Table* table2 = ParentEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            QueryClause queryClause3(QueryClause::parent, synonymS1, integer5);
            Table* table3 = ParentEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause4(QueryClause::parent, synonymS1, integer6);
            Table* table4 = ParentEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->size() == 1);
            REQUIRE(table4->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause5(QueryClause::parent, synonymS1, integer8);
            Table* table5 = ParentEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->size() == 1);
            REQUIRE(table5->getColumn("s1") == unordered_set<string>({"6"}));
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::parent, synonymS1, synonymS2);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 6);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "6"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"3", "4", "5", "6", "7", "8"}));
            delete table1;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::parent, synonymS1, wildcard);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "6"}));
            delete table1;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::parent, wildcard, integer1);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parent, wildcard, integer2);
            Table* table2 = ParentEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            QueryClause queryClause3(QueryClause::parent, wildcard, integer4);
            Table* table3 = ParentEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::parent, wildcard, integer6);
            Table* table4 = ParentEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::parent, wildcard, integer8);
            Table* table5 = ParentEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::TrueTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::parent, wildcard, synonymS1);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 6);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"3", "4", "5", "6", "7", "8"}));
            delete table1;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::parent, wildcard, wildcard);
            Table* table1 = ParentEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);
            delete table1;
        }
    }

    SECTION("ParentT Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::parentT, integer1, integer2);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parentT, integer2, integer3);
            Table* table2 = ParentTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::parentT, integer2, integer6);
            Table* table3 = ParentTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::parentT, integer2, integer7);
            Table* table4 = ParentTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::parentT, integer2, integer8);
            Table* table5 = ParentTEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::TrueTable);

            QueryClause queryClause6(QueryClause::parentT, integer2, integer9);
            Table* table6 = ParentTEvaluator(pkbManager).evaluate(queryClause6);
            REQUIRE(table6->getType() == Table::FalseTable);

            QueryClause queryClause7(QueryClause::parentT, integer6, integer9);
            Table* table7 = ParentTEvaluator(pkbManager).evaluate(queryClause7);
            REQUIRE(table7->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
            delete table7;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::parentT, integer1, synonymS1);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parentT, integer2, synonymS1);
            Table* table2 = ParentTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 6); // includes the nested block
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"3", "4", "5", "6", "7", "8"}));

            QueryClause queryClause3(QueryClause::parentT, integer4, synonymS1);
            Table* table3 = ParentTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::parentT, integer6, synonymS1);
            Table* table4 = ParentTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->size() == 2);
            REQUIRE(table4->getColumn("s1") == unordered_set<string>({"7", "8"}));
            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::parentT, integer1, wildcard);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parentT, integer2, wildcard);
            Table* table2 = ParentTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::parentT, integer5, wildcard);
            Table* table3 = ParentTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::parentT, integer6, wildcard);
            Table* table4 = ParentTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::parentT, integer8, wildcard);
            Table* table5 = ParentTEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::FalseTable);

            QueryClause queryClause6(QueryClause::parentT, integer9, wildcard);
            Table* table6 = ParentTEvaluator(pkbManager).evaluate(queryClause6);
            REQUIRE(table6->getType() == Table::FalseTable);

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::parentT, synonymS1, integer1);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parentT, synonymS1, integer2);
            Table* table2 = ParentTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            QueryClause queryClause3(QueryClause::parentT, synonymS1, integer3);
            Table* table3 = ParentTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause4(QueryClause::parentT, synonymS1, integer4);
            Table* table4 = ParentTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->size() == 1);
            REQUIRE(table4->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause5(QueryClause::parentT, synonymS1, integer6);
            Table* table5 = ParentTEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->size() == 1);
            REQUIRE(table5->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause6(QueryClause::parentT, synonymS1, integer8);
            Table* table6 = ParentTEvaluator(pkbManager).evaluate(queryClause6);
            REQUIRE(table6->size() == 2);
            REQUIRE(table6->getColumn("s1") == unordered_set<string>({"2", "6"}));

            QueryClause queryClause7(QueryClause::parentT, synonymS1, integer9);
            Table* table7 = ParentTEvaluator(pkbManager).evaluate(queryClause7);
            REQUIRE(table7->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
            delete table7;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::parentT, synonymS1, synonymS2);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 8);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "6"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"3", "4", "5", "6", "7", "8"}));
            delete table1;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::parentT, synonymS1, wildcard);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "6"}));
            delete table1;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::parentT, wildcard, integer1);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::parentT, wildcard, integer2);
            Table* table2 = ParentTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            QueryClause queryClause3(QueryClause::parentT, wildcard, integer3);
            Table* table3 = ParentTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::parentT, wildcard, integer4);
            Table* table4 = ParentTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::parentT, wildcard, integer6);
            Table* table5 = ParentTEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->getType() == Table::TrueTable);

            QueryClause queryClause6(QueryClause::parentT, wildcard, integer8);
            Table* table6 = ParentTEvaluator(pkbManager).evaluate(queryClause6);
            REQUIRE(table6->getType() == Table::TrueTable);

            QueryClause queryClause7(QueryClause::parentT, wildcard, integer9);
            Table* table7 = ParentTEvaluator(pkbManager).evaluate(queryClause7);
            REQUIRE(table7->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
            delete table7;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::parentT, wildcard, synonymS1);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 6);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"3", "4", "5", "6", "7", "8"}));
            delete table1;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::parentT, wildcard, wildcard);
            Table* table1 = ParentTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);
            delete table1;
        }
    }

    delete pkbManager;
}
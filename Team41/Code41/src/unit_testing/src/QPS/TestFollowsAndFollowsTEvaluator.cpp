#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/FollowsTable.h"
#include "QPS/Evaluator/FollowsEvaluator.h"
#include "QPS/Evaluator/FollowsTEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: Follows and FollowsT evaluator") {
    string stmt[] = {"1", "2", "3", "4", "5", "6"};
    auto* pkbManager = new PKBManager();

    // 1 <- 2 <- 5 <- 6
    // 3 <- 4 (Think of it as an if block)
    pkbManager->registerFollows(stmt[1], stmt[0]); // 2 -> 1
    pkbManager->registerFollows(stmt[4], stmt[1]); // 5 -> 2
    pkbManager->registerFollows(stmt[5], stmt[4]); // 6 -> 5
    pkbManager->registerFollows(stmt[3], stmt[2]); // 4 -> 3

    pkbManager->registerFollowsT(stmt[1], stmt[0]);
    pkbManager->registerFollowsT(stmt[4], stmt[1]);
    pkbManager->registerFollowsT(stmt[5], stmt[4]);
    pkbManager->registerFollowsT(stmt[3], stmt[2]);

    pkbManager->registerFollowsT(stmt[4], stmt[0]); // 5 -> 1
    pkbManager->registerFollowsT(stmt[5], stmt[0]); // 6 -> 1
    pkbManager->registerFollowsT(stmt[5], stmt[1]); // 6 -> 2

    ClauseVariable integer1(ClauseVariable::variable_type::integer, "1", QueryDeclaration::CONSTANT);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, "2", QueryDeclaration::CONSTANT);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, "3", QueryDeclaration::CONSTANT);
    ClauseVariable integer4(ClauseVariable::variable_type::integer, "4", QueryDeclaration::CONSTANT);
    ClauseVariable integer5(ClauseVariable::variable_type::integer, "5", QueryDeclaration::CONSTANT);
    ClauseVariable integer6(ClauseVariable::variable_type::integer, "6", QueryDeclaration::CONSTANT);

    ClauseVariable synonymS1(ClauseVariable::variable_type::synonym, "s1", QueryDeclaration::ASSIGN);
    ClauseVariable synonymS2(ClauseVariable::variable_type::synonym, "s2", QueryDeclaration::READ);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    pkbManager->registerAssignStmt(stmt[0]);
    pkbManager->registerAssignStmt(stmt[1]);
    pkbManager->registerAssignStmt(stmt[2]);
    pkbManager->registerReadStmt(stmt[3], "re");
    pkbManager->registerAssignStmt(stmt[4]);
    pkbManager->registerReadStmt(stmt[5], "re");

    SECTION("Follows Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::follows, integer1, integer2);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::follows, integer2, integer1);
            Table* table2 = FollowsEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::follows, integer4, integer3);
            Table* table3 = FollowsEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::follows, integer6, integer5);
            Table* table4 = FollowsEvaluator(pkbManager).evaluate(&queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::follows, integer6, integer1);
            Table* table5 = FollowsEvaluator(pkbManager).evaluate(&queryClause5);
            REQUIRE(table5->getType() == Table::FalseTable);

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, integer1, synonymS1);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause2(QueryClause::follows, integer6, synonymS1);
            Table* table2 = FollowsEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->size() == 0);

            delete table1;
            delete table2;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, integer3, wildcard);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::follows, integer6, wildcard);
            Table* table2 = FollowsEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            delete table1;
            delete table2;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, integer3);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::follows, synonymS1, integer2);
            Table* table2 = FollowsEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"1"}));

            QueryClause queryClause3(QueryClause::follows, synonymS1, integer4);
            Table* table3 = FollowsEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"3"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, synonymS2);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"3", "5"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"4", "6"}));
            delete table1;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, wildcard);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
            delete table1;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, integer1);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::follows, wildcard, integer5);
            Table* table2 = FollowsEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::follows, wildcard, integer6);
            Table* table3 = FollowsEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, synonymS1);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2",  "5"}));
            delete table1;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, wildcard);
            Table* table1 = FollowsEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);
            delete table1;
        }
    }

    SECTION("FollowsT Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, integer1, integer2);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::followsT, integer2, integer1);
            Table* table2 = FollowsTEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::followsT, integer4, integer3);
            Table* table3 = FollowsTEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::followsT, integer6, integer5);
            Table* table4 = FollowsTEvaluator(pkbManager).evaluate(&queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);

            // transitive
            QueryClause queryClause5(QueryClause::followsT, integer6, integer1);
            Table* table5 = FollowsTEvaluator(pkbManager).evaluate(&queryClause5);
            REQUIRE(table5->getType() == Table::TrueTable);

            // doesn't follow into an if block
            QueryClause queryClause6(QueryClause::followsT, integer6, integer3);
            Table* table6 = FollowsTEvaluator(pkbManager).evaluate(&queryClause6);
            REQUIRE(table6->getType() == Table::FalseTable);

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, integer1, synonymS1);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "5"}));

            QueryClause queryClause2(QueryClause::followsT, integer6, synonymS1);
            Table* table2 = FollowsTEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->size() == 0);
            delete table1;
            delete table2;
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, integer3, wildcard);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::followsT, integer6, wildcard);
            Table* table2 = FollowsTEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::FalseTable);

            delete table1;
            delete table2;
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, integer3);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::followsT, synonymS1, integer6);
            Table* table2 = FollowsTEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->size() == 3);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"1", "2", "5"}));

            QueryClause queryClause3(QueryClause::followsT, synonymS1, integer4);
            Table* table3 = FollowsTEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"3"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, synonymS2);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 4); // this is larger due to transitive
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"4", "6"}));
            delete table1;
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, wildcard);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 4); // is the same as follows unlike above
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
            delete table1;
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, integer1);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::followsT, wildcard, integer5);
            Table* table2 = FollowsTEvaluator(pkbManager).evaluate(&queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::followsT, wildcard, integer6);
            Table* table3 = FollowsTEvaluator(pkbManager).evaluate(&queryClause3);
            REQUIRE(table3->getType() == Table::TrueTable);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, synonymS1);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2",  "5"}));
            delete table1;
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, wildcard);
            Table* table1 = FollowsTEvaluator(pkbManager).evaluate(&queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);
            delete table1;
        }
    }

    delete pkbManager;
}
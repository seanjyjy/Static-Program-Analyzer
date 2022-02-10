#include "catch.hpp"
#include "PKB/PKB.h"
#include "PKB/FollowsTable.h"
#include "../UnitTestUtility.h"
#include "QPS/Evaluator/FollowsEvaluator.h"
#include "QPS/Evaluator/FollowsTEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: Follows and FollowsT evaluator") {
    auto* table = new FollowsTable();
    string stmt[] = {"1", "2", "3", "4", "5", "6"};
    auto* pkbManager = new PKB();
    pkbManager->followsTable = table;

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

    ClauseVariable integer1(ClauseVariable::variable_type::integer, "1");
    ClauseVariable integer2(ClauseVariable::variable_type::integer, "2");
    ClauseVariable integer3(ClauseVariable::variable_type::integer, "3");
    ClauseVariable integer4(ClauseVariable::variable_type::integer, "4");
    ClauseVariable integer5(ClauseVariable::variable_type::integer, "5");
    ClauseVariable integer6(ClauseVariable::variable_type::integer, "6");

    ClauseVariable synonymS1(ClauseVariable::variable_type::synonym, "s1");
    ClauseVariable synonymS2(ClauseVariable::variable_type::synonym, "s2");

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_");

    SECTION("Follows Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::follows, integer1, integer2);
            REQUIRE(FollowsEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::follows, integer2, integer1);
            REQUIRE(FollowsEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::follows, integer4, integer3);
            REQUIRE(FollowsEvaluator::evaluate(queryClause3, pkbManager) == TrueTable::getTable());

            QueryClause queryClause4(QueryClause::follows, integer6, integer5);
            REQUIRE(FollowsEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());

            QueryClause queryClause5(QueryClause::follows, integer6, integer1);
            REQUIRE(FollowsEvaluator::evaluate(queryClause5, pkbManager) == FalseTable::getTable());
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, integer1, synonymS1);
            Table* table1 = FollowsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2"}));

            QueryClause queryClause2(QueryClause::follows, integer6, synonymS1);
            Table* table2 = FollowsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 0);
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, integer3, wildcard);
            REQUIRE(FollowsEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::follows, integer6, wildcard);
            REQUIRE(FollowsEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, integer3);
            Table* table1 = FollowsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::follows, synonymS1, integer2);
            Table* table2 = FollowsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"1"}));

            QueryClause queryClause3(QueryClause::follows, synonymS1, integer4);
            Table* table3 = FollowsEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"3"}));
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, synonymS2);
            Table* table1 = FollowsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"2", "4", "5", "6"}));
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, synonymS1, wildcard);
            Table* table1 = FollowsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, integer1);
            REQUIRE(FollowsEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::follows, wildcard, integer5);
            REQUIRE(FollowsEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::follows, wildcard, integer6);
            REQUIRE(FollowsEvaluator::evaluate(queryClause3, pkbManager) == TrueTable::getTable());
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, synonymS1);
            Table* table1 = FollowsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "4", "5", "6"}));
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::follows, wildcard, wildcard);
            REQUIRE(FollowsEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());
        }
    }

    SECTION("FollowsT Evaluator") {
        SECTION("Integer Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, integer1, integer2);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::followsT, integer2, integer1);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::followsT, integer4, integer3);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause3, pkbManager) == TrueTable::getTable());

            QueryClause queryClause4(QueryClause::followsT, integer6, integer5);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());

            // transitive
            QueryClause queryClause5(QueryClause::followsT, integer6, integer1);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause5, pkbManager) == TrueTable::getTable());

            // doesn't follow into an if block
            QueryClause queryClause6(QueryClause::followsT, integer6, integer3);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause6, pkbManager) == FalseTable::getTable());
        }

        SECTION("Integer Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, integer1, synonymS1);
            Table* table1 = FollowsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "5", "6"}));

            QueryClause queryClause2(QueryClause::followsT, integer6, synonymS1);
            Table* table2 = FollowsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 0);
        }

        SECTION("Integer Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, integer3, wildcard);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::followsT, integer6, wildcard);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, integer3);
            Table* table1 = FollowsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::followsT, synonymS1, integer6);
            Table* table2 = FollowsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 3);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"1", "2", "5"}));

            QueryClause queryClause3(QueryClause::followsT, synonymS1, integer4);
            Table* table3 = FollowsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("s1") == unordered_set<string>({"3"}));
        }

        SECTION("Synonym Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, synonymS2);
            Table* table1 = FollowsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 7); // this is larger due to transitive
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"2", "4", "5", "6"}));
        }

        SECTION("Synonym Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, synonymS1, wildcard);
            Table* table1 = FollowsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4); // is the same as follows unlike above
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2", "3", "5"}));
        }

        SECTION("Wildcard Integer pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, integer1);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::followsT, wildcard, integer5);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::followsT, wildcard, integer6);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause3, pkbManager) == TrueTable::getTable());
        }

        SECTION("Wildcard Synonym pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, synonymS1);
            Table* table1 = FollowsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"2", "4", "5", "6"}));
        }

        SECTION("Wildcard Wildcard pair") {
            QueryClause queryClause1(QueryClause::followsT, wildcard, wildcard);
            REQUIRE(FollowsTEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());
        }
    }
}
#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/CallsTable.h"
#include "QPS/Evaluator/CallsEvaluator.h"
#include "QPS/Evaluator/CallsTEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: Calls and CallsT evaluator") {
    PKBManager* pkbManager = new PKBManager();
    string proc[] = {"p0", "p1", "p2", "p3", "p4", "p5"};
    string lines[] = {"1", "2"}; // for invalid type of params
    string synonyms[] = { "proc1", "proc2", "proc3" };

    ClauseVariable integer1(ClauseVariable::variable_type::integer, lines[0], QueryDeclaration::CONSTANT);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, lines[1], QueryDeclaration::CONSTANT);

    ClauseVariable proc0(ClauseVariable::variable_type::identifier, proc[0], QueryDeclaration::PROCEDURE);
    ClauseVariable proc1(ClauseVariable::variable_type::identifier, proc[1], QueryDeclaration::PROCEDURE);
    ClauseVariable proc2(ClauseVariable::variable_type::identifier, proc[2], QueryDeclaration::PROCEDURE);
    ClauseVariable proc3(ClauseVariable::variable_type::identifier, proc[3], QueryDeclaration::PROCEDURE);
    ClauseVariable proc4(ClauseVariable::variable_type::identifier, proc[4], QueryDeclaration::PROCEDURE);
    ClauseVariable proc5(ClauseVariable::variable_type::identifier, proc[5], QueryDeclaration::PROCEDURE);

    ClauseVariable procSyn1(ClauseVariable::variable_type::synonym, synonyms[0], QueryDeclaration::PROCEDURE);
    ClauseVariable procSyn2(ClauseVariable::variable_type::synonym, synonyms[1], QueryDeclaration::PROCEDURE);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    // Register Calls
    /*
            0
           / \
          1   2
         /\   |
        /  \  5
       /    \|
       3     4
     */

    // p0 -> {p1, p2}
    pkbManager->registerCalls(proc[0], proc[1]);
    pkbManager->registerCalls(proc[0], proc[2]);
    // p1 -> {p3, p4}
    pkbManager->registerCalls(proc[1], proc[3]);
    pkbManager->registerCalls(proc[1], proc[4]);
    // p2 -> p5
    pkbManager->registerCalls(proc[2], proc[5]);
    // p5 -> p4
    pkbManager->registerCalls(proc[5], proc[4]);

    // Register CallsT
    pkbManager->registerCallsT(proc[0], proc[1]);
    pkbManager->registerCallsT(proc[0], proc[2]);
    pkbManager->registerCallsT(proc[0], proc[3]);
    pkbManager->registerCallsT(proc[0], proc[4]);
    pkbManager->registerCallsT(proc[0], proc[5]);
    pkbManager->registerCallsT(proc[1], proc[3]);
    pkbManager->registerCallsT(proc[1], proc[4]);
    pkbManager->registerCallsT(proc[2], proc[4]);
    pkbManager->registerCallsT(proc[2], proc[5]);
    pkbManager->registerCallsT(proc[5], proc[4]);

    SECTION("Calls Evaluator") {
        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::calls, proc0, proc3);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::calls, proc0, proc1);
            Table* table2 = CallsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::calls, proc2, proc5);
            Table* table3 = CallsEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::calls, proc5, proc4);
            Table* table4 = CallsEvaluator::evaluate(queryClause4, pkbManager);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::calls, proc2, proc4);
            Table* table5 = CallsEvaluator::evaluate(queryClause5, pkbManager);
            REQUIRE(table5->getType() == Table::FalseTable);

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, proc0, procSyn1);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2"}));

            QueryClause queryClause2(QueryClause::calls, proc2, procSyn1);
            Table* table2 = CallsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p5"}));

            QueryClause queryClause3(QueryClause::calls, proc3, procSyn1);
            Table* table3 = CallsEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 0);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, proc0, wildcard);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::calls, proc4, wildcard);
            Table* table2 = CallsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::FalseTable);

            delete table1;
            delete table2;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, proc0);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::calls, procSyn1, proc4);
            Table* table2 = CallsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p1", "p5"}));

            QueryClause queryClause3(QueryClause::calls, procSyn1, proc5);
            Table* table3 = CallsEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p2"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, procSyn2);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 6);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));
            REQUIRE(table1->getColumn("proc2") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, wildcard);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            delete table1;
        }

        SECTION("WildCard Identifier Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, proc0);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::calls, wildcard, proc3);
            Table* table2 = CallsEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::calls, wildcard, proc4);
            Table* table3 = CallsEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->getType() == Table::TrueTable);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("WildCard Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, procSyn1);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("WildCard WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, wildcard);
            Table* table1 = CallsEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::TrueTable);

            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::calls, integer1, wildcard);
            REQUIRE_THROWS(CallsEvaluator::evaluate(queryClause1, pkbManager));

            QueryClause queryClause2(QueryClause::calls, procSyn1,integer1);
            REQUIRE_THROWS(CallsEvaluator::evaluate(queryClause2, pkbManager));

            QueryClause queryClause3(QueryClause::calls, integer1, integer2);
            REQUIRE_THROWS(CallsEvaluator::evaluate(queryClause3, pkbManager));
        }
    }

    SECTION("CallsT Evaluator") {
        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, proc3);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::callsT, proc0, proc1);
            Table* table2 = CallsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::callsT, proc2, proc5);
            Table* table3 = CallsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->getType() == Table::TrueTable);

            QueryClause queryClause4(QueryClause::callsT, proc5, proc4);
            Table* table4 = CallsTEvaluator::evaluate(queryClause4, pkbManager);
            REQUIRE(table4->getType() == Table::TrueTable);

            QueryClause queryClause5(QueryClause::callsT, proc2, proc4);
            Table* table5 = CallsTEvaluator::evaluate(queryClause5, pkbManager);
            REQUIRE(table5->getType() == Table::TrueTable);

            QueryClause queryClause6(QueryClause::callsT, proc4, proc3);
            Table* table6 = CallsTEvaluator::evaluate(queryClause6, pkbManager);
            REQUIRE(table6->getType() == Table::FalseTable);

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, procSyn1);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            QueryClause queryClause2(QueryClause::callsT, proc1, procSyn1);
            Table* table2 = CallsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p3", "p4"}));

            QueryClause queryClause3(QueryClause::callsT, proc2, procSyn1);
            Table* table3 = CallsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 2);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p4", "p5"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, wildcard);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::callsT, proc3, wildcard);
            Table* table2 = CallsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::FalseTable);

            QueryClause queryClause3(QueryClause::callsT, proc2, wildcard);
            Table* table3 = CallsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->getType() == Table::TrueTable);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, proc0);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::callsT, procSyn1, proc4);
            Table* table2 = CallsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 4);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            QueryClause queryClause3(QueryClause::callsT, procSyn1, proc5);
            Table* table3 = CallsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->size() == 2);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p0", "p2"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, procSyn2);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 10);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));
            REQUIRE(table1->getColumn("proc2") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, wildcard);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            delete table1;
        }

        SECTION("WildCard Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, proc0);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::FalseTable);

            QueryClause queryClause2(QueryClause::callsT, wildcard, proc3);
            Table* table2 = CallsTEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::callsT, wildcard, proc4);
            Table* table3 = CallsTEvaluator::evaluate(queryClause3, pkbManager);
            REQUIRE(table3->getType() == Table::TrueTable);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("WildCard Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, procSyn1);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("WildCard WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, wildcard);
            Table* table1 = CallsTEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->getType() == Table::TrueTable);

            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::callsT, integer1, wildcard);
            REQUIRE_THROWS(CallsTEvaluator::evaluate(queryClause1, pkbManager));

            QueryClause queryClause2(QueryClause::callsT, procSyn1,integer1);
            REQUIRE_THROWS(CallsTEvaluator::evaluate(queryClause2, pkbManager));

            QueryClause queryClause3(QueryClause::callsT, integer1, integer2);
            REQUIRE_THROWS(CallsTEvaluator::evaluate(queryClause3, pkbManager));
        }
    }

    delete pkbManager;
}
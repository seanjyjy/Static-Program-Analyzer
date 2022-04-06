#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/CallsTable.h"
#include "QPS/Evaluator/CallsEvaluator.h"
#include "QPS/Evaluator/CallsTEvaluator.h"
#include "QPS/QO/ClauseVariable.h"

TEST_CASE("Evaluator: Calls and CallsT evaluator") {
    PKBManager* pkbManager = new PKBManager();
    string proc[] = {"p0", "p1", "p2", "p3", "p4", "p5"};
    string stmt[] = {"1", "2", "3", "4", "5", "6"};
    string lines[] = {"1", "2"}; // for invalid type of params
    string synonyms[] = { "proc1", "proc2", "proc3" };

    ProcedureEntities procedureEntities;
    ConstantEntities constantEntities;

    ClauseVariable integer1(ClauseVariable::variable_type::integer, lines[0], &constantEntities);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, lines[1], &constantEntities);

    ClauseVariable proc0(ClauseVariable::variable_type::identifier, proc[0], &procedureEntities);
    ClauseVariable proc1(ClauseVariable::variable_type::identifier, proc[1], &procedureEntities);
    ClauseVariable proc2(ClauseVariable::variable_type::identifier, proc[2], &procedureEntities);
    ClauseVariable proc3(ClauseVariable::variable_type::identifier, proc[3], &procedureEntities);
    ClauseVariable proc4(ClauseVariable::variable_type::identifier, proc[4], &procedureEntities);
    ClauseVariable proc5(ClauseVariable::variable_type::identifier, proc[5], &procedureEntities);

    ClauseVariable procSyn1(ClauseVariable::variable_type::synonym, synonyms[0], &procedureEntities);
    ClauseVariable procSyn2(ClauseVariable::variable_type::synonym, synonyms[1], &procedureEntities);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", nullptr);

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

    for (auto& p : proc) {
        pkbManager->registerProcedure(p);
    }

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
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isFalseTable());

            QueryClause queryClause2(QueryClause::calls, proc0, proc1);
            Table* table2 = CallsEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::calls, proc2, proc5);
            Table* table3 = CallsEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::calls, proc5, proc4);
            Table* table4 = CallsEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            QueryClause queryClause5(QueryClause::calls, proc2, proc4);
            Table* table5 = CallsEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->isFalseTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, proc0, procSyn1);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2"}));

            QueryClause queryClause2(QueryClause::calls, proc2, procSyn1);
            Table* table2 = CallsEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p5"}));

            QueryClause queryClause3(QueryClause::calls, proc3, procSyn1);
            Table* table3 = CallsEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 0);

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, proc0, wildcard);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::calls, proc4, wildcard);
            Table* table2 = CallsEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isFalseTable());

            delete table1;
            delete table2;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, proc0);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::calls, procSyn1, proc4);
            Table* table2 = CallsEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p1", "p5"}));

            QueryClause queryClause3(QueryClause::calls, procSyn1, proc5);
            Table* table3 = CallsEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 1);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p2"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, procSyn2);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 6);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));
            REQUIRE(table1->getColumn("proc2") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, procSyn1, wildcard);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            delete table1;
        }

        SECTION("WildCard Identifier Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, proc0);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isFalseTable());

            QueryClause queryClause2(QueryClause::calls, wildcard, proc3);
            Table* table2 = CallsEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::calls, wildcard, proc4);
            Table* table3 = CallsEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("WildCard Synonym Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, procSyn1);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("WildCard WildCard Pair") {
            QueryClause queryClause1(QueryClause::calls, wildcard, wildcard);
            Table* table1 = CallsEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::calls, integer1, wildcard);
            REQUIRE_THROWS(CallsEvaluator(pkbManager).evaluate(queryClause1));

            QueryClause queryClause2(QueryClause::calls, procSyn1,integer1);
            REQUIRE_THROWS(CallsEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause3(QueryClause::calls, integer1, integer2);
            REQUIRE_THROWS(CallsEvaluator(pkbManager).evaluate(queryClause3));
        }
    }

    SECTION("CallsT Evaluator") {
        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, proc3);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::callsT, proc0, proc1);
            Table* table2 = CallsTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::callsT, proc2, proc5);
            Table* table3 = CallsTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            QueryClause queryClause4(QueryClause::callsT, proc5, proc4);
            Table* table4 = CallsTEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            QueryClause queryClause5(QueryClause::callsT, proc2, proc4);
            Table* table5 = CallsTEvaluator(pkbManager).evaluate(queryClause5);
            REQUIRE(table5->isTrueTable());

            QueryClause queryClause6(QueryClause::callsT, proc4, proc3);
            Table* table6 = CallsTEvaluator(pkbManager).evaluate(queryClause6);
            REQUIRE(table6->isFalseTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
            delete table6;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, procSyn1);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            QueryClause queryClause2(QueryClause::callsT, proc1, procSyn1);
            Table* table2 = CallsTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p3", "p4"}));

            QueryClause queryClause3(QueryClause::callsT, proc2, procSyn1);
            Table* table3 = CallsTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 2);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p4", "p5"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, proc0, wildcard);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::callsT, proc3, wildcard);
            Table* table2 = CallsTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isFalseTable());

            QueryClause queryClause3(QueryClause::callsT, proc2, wildcard);
            Table* table3 = CallsTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, proc0);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 0);

            QueryClause queryClause2(QueryClause::callsT, procSyn1, proc4);
            Table* table2 = CallsTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 4);
            REQUIRE(table2->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            QueryClause queryClause3(QueryClause::callsT, procSyn1, proc5);
            Table* table3 = CallsTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->size() == 2);
            REQUIRE(table3->getColumn("proc1") == unordered_set<string>({"p0", "p2"}));

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, procSyn2);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 10);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));
            REQUIRE(table1->getColumn("proc2") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, procSyn1, wildcard);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 4);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p0", "p1", "p2", "p5"}));

            delete table1;
        }

        SECTION("WildCard Identifier Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, proc0);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isFalseTable());

            QueryClause queryClause2(QueryClause::callsT, wildcard, proc3);
            Table* table2 = CallsTEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::callsT, wildcard, proc4);
            Table* table3 = CallsTEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("WildCard Synonym Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, procSyn1);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 5);
            REQUIRE(table1->getColumn("proc1") == unordered_set<string>({"p1", "p2", "p3", "p4", "p5"}));

            delete table1;
        }

        SECTION("WildCard WildCard Pair") {
            QueryClause queryClause1(QueryClause::callsT, wildcard, wildcard);
            Table* table1 = CallsTEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::callsT, integer1, wildcard);
            REQUIRE_THROWS(CallsTEvaluator(pkbManager).evaluate(queryClause1));

            QueryClause queryClause2(QueryClause::callsT, procSyn1,integer1);
            REQUIRE_THROWS(CallsTEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause3(QueryClause::callsT, integer1, integer2);
            REQUIRE_THROWS(CallsTEvaluator(pkbManager).evaluate(queryClause3));
        }
    }

    delete pkbManager;
}
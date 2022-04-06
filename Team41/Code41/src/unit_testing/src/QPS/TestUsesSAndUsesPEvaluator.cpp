#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/UsesTable.h"
#include "QPS/Evaluator/UsesSEvaluator.h"
#include "QPS/Evaluator/UsesPEvaluator.h"
#include "QPS/QO/ClauseVariable.h"

TEST_CASE("Evaluator: UsesS and UsesP evaluator") {
    PKBManager* pkbManager = new PKBManager();
    string vars[] = {"var1", "var2"};
    string lines[] = {"1", "2", "3"};
    string proc[] = {"proc1", "proc2", "proc3"};
    string declaration[] = { "a", "v", "p", "r" };

    AssignEntities assignEntities;
    ReadEntities readEntities;
    ProcedureEntities procedureEntities;
    ConstantEntities constantEntities;
    VariableEntities variableEntities;

    ClauseVariable integer1(ClauseVariable::variable_type::integer, lines[0], &constantEntities);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, lines[1], &constantEntities);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, lines[2], &constantEntities);

    ClauseVariable proc1(ClauseVariable::variable_type::identifier, proc[0], &procedureEntities);
    ClauseVariable proc2(ClauseVariable::variable_type::identifier, proc[1], &procedureEntities);
    ClauseVariable proc3(ClauseVariable::variable_type::identifier, proc[2], &procedureEntities);

    ClauseVariable identifierVar1(ClauseVariable::variable_type::identifier, vars[0], &variableEntities);
    ClauseVariable identifierVar2(ClauseVariable::variable_type::identifier, vars[1], &variableEntities);

    ClauseVariable assignSyn(ClauseVariable::variable_type::synonym, declaration[0], &assignEntities);
    ClauseVariable varSyn(ClauseVariable::variable_type::synonym, declaration[1], &variableEntities);
    ClauseVariable procSyn(ClauseVariable::variable_type::synonym, declaration[2], &procedureEntities);
    ClauseVariable readSyn(ClauseVariable::variable_type::synonym, declaration[3], &readEntities);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", nullptr);

    pkbManager->registerVariable(vars[0]);
    pkbManager->registerVariable(vars[1]);
    pkbManager->registerAssignStmt(lines[0]);
    pkbManager->registerAssignStmt(lines[1]);
    pkbManager->registerUsesS(lines[0], vars[0]);
    pkbManager->registerUsesS(lines[0], declaration[1]);
    pkbManager->registerUsesS(lines[1], declaration[1]);
    pkbManager->registerProcedure(proc[0]);
    pkbManager->registerProcedure(proc[1]);
    pkbManager->registerProcedure(proc[2]);

    SECTION("UsesS Evaluator") {
        pkbManager->registerUsesS(lines[0], vars[0]);
        pkbManager->registerUsesS(lines[0], vars[1]);
        pkbManager->registerUsesS(lines[1], vars[1]);

        SECTION("Integer Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, identifierVar1);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::usesS, integer1, identifierVar2);
            Table* table2 = UsesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::usesS, integer2, identifierVar1);
            Table* table3 = UsesSEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isFalseTable());

            QueryClause queryClause4(QueryClause::usesS, integer2, identifierVar2);
            Table* table4 = UsesSEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Integer Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, varSyn);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::usesS, integer2, varSyn);
            Table* table2 = UsesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
            delete table1;
            delete table2;
        }

        SECTION("Integer WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, wildcard);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::usesS, integer2, wildcard);
            Table* table2 = UsesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::usesS, integer3, wildcard);
            Table* table3 = UsesSEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isFalseTable());
            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, identifierVar1);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1"}));

            QueryClause queryClause2(QueryClause::usesS, assignSyn, identifierVar2);
            Table* table2 = UsesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("a") == unordered_set<string>({"1", "2"}));
            delete table1;
            delete table2;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, varSyn);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, wildcard);
            Table* table1 = UsesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause2(QueryClause::usesS, wildcard, varSyn);
            REQUIRE_THROWS(UsesSEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause4(QueryClause::modifiesS, procSyn, assignSyn);
            REQUIRE_THROWS(UsesSEvaluator(pkbManager).evaluate(queryClause4));

            QueryClause queryClause5(QueryClause::modifiesS, procSyn, wildcard);
            REQUIRE_THROWS(UsesSEvaluator(pkbManager).evaluate(queryClause5));

            QueryClause queryClause6(QueryClause::modifiesS, readSyn, wildcard);
            REQUIRE_THROWS(UsesSEvaluator(pkbManager).evaluate(queryClause6));
        }
    }

    SECTION("UsesP Evaluator") {
        pkbManager->registerUsesP(proc[0], vars[0]);
        pkbManager->registerUsesP(proc[0], vars[1]);
        pkbManager->registerUsesP(proc[1], vars[1]);

        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, identifierVar1);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::usesP, proc1, identifierVar2);
            Table* table2 = UsesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::usesP, proc2, identifierVar1);
            Table* table3 = UsesPEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isFalseTable());

            QueryClause queryClause4(QueryClause::usesP, proc2, identifierVar2);
            Table* table4 = UsesPEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->isTrueTable());
            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, varSyn);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::usesP, proc2, varSyn);
            Table* table2 = UsesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
            delete table1;
            delete table2;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, wildcard);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->isTrueTable());

            QueryClause queryClause2(QueryClause::usesP, proc2, wildcard);
            Table* table2 = UsesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->isTrueTable());

            QueryClause queryClause3(QueryClause::usesP, proc3, wildcard);
            Table* table3 = UsesPEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->isFalseTable());
            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, identifierVar1);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1"}));

            QueryClause queryClause2(QueryClause::usesP, procSyn, identifierVar2);
            Table* table2 = UsesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            delete table1;
            delete table2;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, varSyn);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, wildcard);
            Table* table1 = UsesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause2(QueryClause::usesS, wildcard, varSyn );
            REQUIRE_THROWS(UsesPEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause4(QueryClause::modifiesS, assignSyn, assignSyn);
            REQUIRE_THROWS(UsesPEvaluator(pkbManager).evaluate(queryClause4));

            QueryClause queryClause5(QueryClause::modifiesS, assignSyn, wildcard);
            REQUIRE_THROWS(UsesPEvaluator(pkbManager).evaluate(queryClause5));
        }
    }
    delete pkbManager;
}
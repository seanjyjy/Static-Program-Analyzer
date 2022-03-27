#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/ModifiesTable.h"
#include "QPS/Evaluator/ModifiesSEvaluator.h"
#include "QPS/Evaluator/ModifiesPEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: ModifiesS and ModifiesP evaluator") {
    PKBManager* pkbManager = new PKBManager();
    string vars[] = {"var1", "var2"};
    string lines[] = {"1", "2", "3"};
    string proc[] = {"proc1", "proc2", "proc3"};
    string declaration[] = { "a", "v", "p", "pn", "c" };

    ClauseVariable integer1(ClauseVariable::variable_type::integer, lines[0], QueryDeclaration::CONSTANT);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, lines[1], QueryDeclaration::CONSTANT);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, lines[2], QueryDeclaration::CONSTANT);

    ClauseVariable proc1(ClauseVariable::variable_type::identifier, proc[0], QueryDeclaration::PROCEDURE);
    ClauseVariable proc2(ClauseVariable::variable_type::identifier, proc[1], QueryDeclaration::PROCEDURE);
    ClauseVariable proc3(ClauseVariable::variable_type::identifier, proc[2], QueryDeclaration::PROCEDURE);

    ClauseVariable identifierVar1(ClauseVariable::variable_type::identifier, vars[0], QueryDeclaration::VARIABLE);
    ClauseVariable identifierVar2(ClauseVariable::variable_type::identifier, vars[1], QueryDeclaration::VARIABLE);

    ClauseVariable assignSyn(ClauseVariable::variable_type::synonym, declaration[0], QueryDeclaration::ASSIGN);
    ClauseVariable varSyn(ClauseVariable::variable_type::synonym, declaration[1], QueryDeclaration::VARIABLE);
    ClauseVariable procSyn(ClauseVariable::variable_type::synonym, declaration[2], QueryDeclaration::PROCEDURE);
    ClauseVariable printSyn(ClauseVariable::variable_type::synonym, declaration[3], QueryDeclaration::PRINT);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    pkbManager->registerVariable(vars[0]);
    pkbManager->registerVariable(vars[1]);
    pkbManager->registerAssignStmt(lines[0]);
    pkbManager->registerAssignStmt(lines[1]);
    pkbManager->registerModifiesS(lines[0], vars[0]);
    pkbManager->registerModifiesS(lines[0], declaration[1]);
    pkbManager->registerModifiesS(lines[1], declaration[1]);
    pkbManager->registerProcedure(proc[0]);
    pkbManager->registerProcedure(proc[1]);
    pkbManager->registerProcedure(proc[2]);

    SECTION("ModifiesS Evaluator") {
        pkbManager->registerModifiesS(lines[0], vars[0]);
        pkbManager->registerModifiesS(lines[0], vars[1]);
        pkbManager->registerModifiesS(lines[1], vars[1]);

        SECTION("Integer Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, identifierVar1);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::modifiesS, integer1, identifierVar2);
            Table* table2 = ModifiesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::modifiesS, integer2, identifierVar1);
            Table* table3 = ModifiesSEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::modifiesS, integer2, identifierVar2);
            Table* table4 = ModifiesSEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Integer Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, varSyn);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::modifiesS, integer2, varSyn);
            Table* table2 = ModifiesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
            delete table1;
            delete table2;
        }

        SECTION("Integer WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, wildcard);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::modifiesS, integer2, wildcard);
            Table* table2 = ModifiesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::modifiesS, integer3, wildcard);
            Table* table3 = ModifiesSEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, assignSyn, identifierVar1);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1"}));

            QueryClause queryClause2(QueryClause::modifiesS, assignSyn, identifierVar2);
            Table* table2 = ModifiesSEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("a") == unordered_set<string>({"1", "2"}));
            delete table1;
            delete table2;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, assignSyn, varSyn);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, assignSyn, wildcard);
            Table* table1 = ModifiesSEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause2(QueryClause::modifiesS, wildcard, assignSyn);
            REQUIRE_THROWS(ModifiesSEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause4(QueryClause::modifiesS, procSyn, assignSyn);
            REQUIRE_THROWS(ModifiesSEvaluator(pkbManager).evaluate(queryClause4));

            QueryClause queryClause5(QueryClause::modifiesS, procSyn, wildcard);
            REQUIRE_THROWS(ModifiesSEvaluator(pkbManager).evaluate(queryClause5));

            QueryClause queryClause6(QueryClause::modifiesS, printSyn, assignSyn);
            REQUIRE_THROWS(ModifiesSEvaluator(pkbManager).evaluate(queryClause6));
        }
    }

    SECTION("ModifiesP Evaluator") {
        pkbManager->registerModifiesP(proc[0], vars[0]);
        pkbManager->registerModifiesP(proc[0], vars[1]);
        pkbManager->registerModifiesP(proc[1], vars[1]);

        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, identifierVar1);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::modifiesP, proc1, identifierVar2);
            Table* table2 = ModifiesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::modifiesP, proc2, identifierVar1);
            Table* table3 = ModifiesPEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);

            QueryClause queryClause4(QueryClause::modifiesP, proc2, identifierVar2);
            Table* table4 = ModifiesPEvaluator(pkbManager).evaluate(queryClause4);
            REQUIRE(table4->getType() == Table::TrueTable);
            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, varSyn);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::modifiesP, proc2, varSyn);
            Table* table2 = ModifiesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
            delete table1;
            delete table2;
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, wildcard);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->getType() == Table::TrueTable);

            QueryClause queryClause2(QueryClause::modifiesP, proc2, wildcard);
            Table* table2 = ModifiesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->getType() == Table::TrueTable);

            QueryClause queryClause3(QueryClause::modifiesP, proc3, wildcard);
            Table* table3 = ModifiesPEvaluator(pkbManager).evaluate(queryClause3);
            REQUIRE(table3->getType() == Table::FalseTable);
            delete table1;
            delete table2;
            delete table3;
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, procSyn, identifierVar1);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1"}));

            QueryClause queryClause2(QueryClause::modifiesP, procSyn, identifierVar2);
            Table* table2 = ModifiesPEvaluator(pkbManager).evaluate(queryClause2);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            delete table1;
            delete table2;
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, procSyn, varSyn);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
            delete table1;
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, procSyn, wildcard);
            Table* table1 = ModifiesPEvaluator(pkbManager).evaluate(queryClause1);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            delete table1;
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause2(QueryClause::modifiesS, wildcard, assignSyn);
            REQUIRE_THROWS(ModifiesPEvaluator(pkbManager).evaluate(queryClause2));

            QueryClause queryClause4(QueryClause::modifiesS, assignSyn, assignSyn);
            REQUIRE_THROWS(ModifiesPEvaluator(pkbManager).evaluate(queryClause4));

            QueryClause queryClause5(QueryClause::modifiesS, assignSyn, wildcard);
            REQUIRE_THROWS(ModifiesPEvaluator(pkbManager).evaluate(queryClause5));
        }
    }

    delete pkbManager;
}
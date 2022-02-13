#include "catch.hpp"
#include "PKB/PKB.h"
#include "PKB/ModifiesTable.h"
#include "../UnitTestUtility.h"
#include "QPS/Evaluator/ModifiesSEvaluator.h"
#include "QPS/Evaluator/ModifiesPEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: ModifiesS and ModifiesP evaluator") {
    ModifiesTable table;
    PKB* pkbManager = new PKB();
    pkbManager->modifiesTable = &table;
    string vars[] = {"var1", "var2"};
    string lines[] = {"1", "2", "3"};
    string proc[] = {"proc1", "proc2", "proc3"};

    ClauseVariable integer1(ClauseVariable::variable_type::integer, lines[0], QueryDeclaration::NONE);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, lines[1], QueryDeclaration::NONE);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, lines[2], QueryDeclaration::NONE);

    ClauseVariable proc1(ClauseVariable::variable_type::identifier, proc[0], QueryDeclaration::NONE);
    ClauseVariable proc2(ClauseVariable::variable_type::identifier, proc[1], QueryDeclaration::NONE);
    ClauseVariable proc3(ClauseVariable::variable_type::identifier, proc[2], QueryDeclaration::NONE);

    ClauseVariable identifierVar1(ClauseVariable::variable_type::identifier, vars[0], QueryDeclaration::NONE);
    ClauseVariable identifierVar2(ClauseVariable::variable_type::identifier, vars[1], QueryDeclaration::NONE);

    ClauseVariable synonymS1(ClauseVariable::variable_type::synonym, "s1", QueryDeclaration::NONE);
    ClauseVariable synonymS2(ClauseVariable::variable_type::synonym, "s2", QueryDeclaration::NONE);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    SECTION("ModifiesS Evaluator") {
        pkbManager->registerModifiesS(lines[0], vars[0]);
        pkbManager->registerModifiesS(lines[0], vars[1]);
        pkbManager->registerModifiesS(lines[1], vars[1]);

        SECTION("Integer Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, identifierVar1);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesS, integer1, identifierVar2);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesS, integer2, identifierVar1);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::modifiesS, integer2, identifierVar2);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());
        }

        SECTION("Integer Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, synonymS1);
            Table* table1 = ModifiesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::modifiesS, integer2, synonymS1);
            Table* table2 = ModifiesSEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"var2"}));
        }

        SECTION("Integer WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, integer1, wildcard);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesS, integer2, wildcard);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesS, integer3, wildcard);
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, synonymS1, identifierVar1);
            Table* table1 = ModifiesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1"}));

            QueryClause queryClause2(QueryClause::modifiesS, synonymS1, identifierVar2);
            Table* table2 = ModifiesSEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"1", "2"}));
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, synonymS1, synonymS2);
            Table* table1 = ModifiesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"var1", "var2"}));
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesS, synonymS1, wildcard);
            Table* table1 = ModifiesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"1", "2"}));
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::modifiesS, wildcard, identifierVar1 );
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesS, wildcard, synonymS1 );
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesS, wildcard, wildcard );
            REQUIRE(ModifiesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }
    }

    SECTION("ModifiesP Evaluator") {
        pkbManager->registerModifiesP(proc[0], vars[0]);
        pkbManager->registerModifiesP(proc[0], vars[1]);
        pkbManager->registerModifiesP(proc[1], vars[1]);

        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, identifierVar1);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesP, proc1, identifierVar2);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesP, proc2, identifierVar1);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::modifiesP, proc2, identifierVar2);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, synonymS1);
            Table* table1 = ModifiesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::modifiesP, proc2, synonymS1);
            Table* table2 = ModifiesPEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"var2"}));
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, proc1, wildcard);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesP, proc2, wildcard);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesP, proc3, wildcard);
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, synonymS1, identifierVar1);
            Table* table1 = ModifiesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"proc1"}));

            QueryClause queryClause2(QueryClause::modifiesP, synonymS1, identifierVar2);
            Table* table2 = ModifiesPEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("s1") == unordered_set<string>({"proc1", "proc2"}));
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, synonymS1, synonymS2);
            Table* table1 = ModifiesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"proc1", "proc2"}));
            REQUIRE(table1->getColumn("s2") == unordered_set<string>({"var1", "var2"}));
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::modifiesP, synonymS1, wildcard);
            Table* table1 = ModifiesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("s1") == unordered_set<string>({"proc1", "proc2"}));
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::modifiesS, wildcard, identifierVar1 );
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::modifiesS, wildcard, synonymS1 );
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());

            QueryClause queryClause3(QueryClause::modifiesS, wildcard, wildcard );
            REQUIRE(ModifiesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }
    }
}
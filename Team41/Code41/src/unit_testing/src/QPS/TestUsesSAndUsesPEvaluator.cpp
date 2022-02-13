#include "catch.hpp"
#include "PKB/PKB.h"
#include "PKB/UsesTable.h"
#include "../UnitTestUtility.h"
#include "QPS/Evaluator/UsesSEvaluator.h"
#include "QPS/Evaluator/UsesPEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: UsesS and UsesP evaluator") {
    UsesTable table;
    PKB* pkbManager = new PKB();
    pkbManager->usesTable = &table;
    string vars[] = {"var1", "var2"};
    string lines[] = {"1", "2", "3"};
    string proc[] = {"proc1", "proc2", "proc3"};
    string declaration[] = { "a", "v", "p" };

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

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    SECTION("UsesS Evaluator") {
        pkbManager->registerUsesS(lines[0], vars[0]);
        pkbManager->registerUsesS(lines[0], vars[1]);
        pkbManager->registerUsesS(lines[1], vars[1]);

        SECTION("Integer Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, identifierVar1);
            REQUIRE(UsesSEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::usesS, integer1, identifierVar2);
            REQUIRE(UsesSEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::usesS, integer2, identifierVar1);
            REQUIRE(UsesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::usesS, integer2, identifierVar2);
            REQUIRE(UsesSEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());
        }

        SECTION("Integer Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, varSyn);
            Table* table1 = UsesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::usesS, integer2, varSyn);
            Table* table2 = UsesSEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
        }

        SECTION("Integer WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesS, integer1, wildcard);
            REQUIRE(UsesSEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::usesS, integer2, wildcard);
            REQUIRE(UsesSEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::usesS, integer3, wildcard);
            REQUIRE(UsesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, identifierVar1);
            Table* table1 = UsesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1"}));

            QueryClause queryClause2(QueryClause::usesS, assignSyn, identifierVar2);
            Table* table2 = UsesSEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("a") == unordered_set<string>({"1", "2"}));
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, varSyn);
            Table* table1 = UsesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesS, assignSyn, wildcard);
            Table* table1 = UsesSEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("a") == unordered_set<string>({"1", "2"}));
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::usesS, wildcard, identifierVar1);
            REQUIRE(UsesSEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::usesS, wildcard, varSyn);
            REQUIRE(UsesSEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());

            QueryClause queryClause3(QueryClause::usesS, wildcard, wildcard);
            REQUIRE(UsesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::modifiesS, procSyn, assignSyn);
            REQUIRE(UsesSEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());

            QueryClause queryClause5(QueryClause::modifiesS, procSyn, wildcard);
            REQUIRE(UsesSEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }
    }

    SECTION("UsesP Evaluator") {
        pkbManager->registerUsesP(proc[0], vars[0]);
        pkbManager->registerUsesP(proc[0], vars[1]);
        pkbManager->registerUsesP(proc[1], vars[1]);

        SECTION("Identifier Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, identifierVar1);
            REQUIRE(UsesPEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::usesP, proc1, identifierVar2);
            REQUIRE(UsesPEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::usesP, proc2, identifierVar1);
            REQUIRE(UsesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::usesP, proc2, identifierVar2);
            REQUIRE(UsesPEvaluator::evaluate(queryClause4, pkbManager) == TrueTable::getTable());
        }

        SECTION("Identifier Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, varSyn);
            Table* table1 = UsesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));

            QueryClause queryClause2(QueryClause::usesP, proc2, varSyn);
            Table* table2 = UsesPEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 1);
            REQUIRE(table2->getColumn("v") == unordered_set<string>({"var2"}));
        }

        SECTION("Identifier WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesP, proc1, wildcard);
            REQUIRE(UsesPEvaluator::evaluate(queryClause1, pkbManager) == TrueTable::getTable());

            QueryClause queryClause2(QueryClause::usesP, proc2, wildcard);
            REQUIRE(UsesPEvaluator::evaluate(queryClause2, pkbManager) == TrueTable::getTable());

            QueryClause queryClause3(QueryClause::usesP, proc3, wildcard);
            REQUIRE(UsesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }

        SECTION("Synonym Identifier Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, identifierVar1);
            Table* table1 = UsesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 1);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1"}));

            QueryClause queryClause2(QueryClause::usesP, procSyn, identifierVar2);
            Table* table2 = UsesPEvaluator::evaluate(queryClause2, pkbManager);
            REQUIRE(table2->size() == 2);
            REQUIRE(table2->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
        }

        SECTION("Synonym Synonym Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, varSyn);
            Table* table1 = UsesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 3);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
            REQUIRE(table1->getColumn("v") == unordered_set<string>({"var1", "var2"}));
        }

        SECTION("Synonym WildCard Pair") {
            QueryClause queryClause1(QueryClause::usesP, procSyn, wildcard);
            Table* table1 = UsesPEvaluator::evaluate(queryClause1, pkbManager);
            REQUIRE(table1->size() == 2);
            REQUIRE(table1->getColumn("p") == unordered_set<string>({"proc1", "proc2"}));
        }

        SECTION("Semantically Invalid") {
            QueryClause queryClause1(QueryClause::usesS, wildcard, identifierVar1 );
            REQUIRE(UsesPEvaluator::evaluate(queryClause1, pkbManager) == FalseTable::getTable());

            QueryClause queryClause2(QueryClause::usesS, wildcard, varSyn );
            REQUIRE(UsesPEvaluator::evaluate(queryClause2, pkbManager) == FalseTable::getTable());

            QueryClause queryClause3(QueryClause::usesS, wildcard, wildcard );
            REQUIRE(UsesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause4(QueryClause::modifiesS, assignSyn, assignSyn);
            REQUIRE(UsesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());

            QueryClause queryClause5(QueryClause::modifiesS, assignSyn, wildcard);
            REQUIRE(UsesPEvaluator::evaluate(queryClause3, pkbManager) == FalseTable::getTable());
        }
    }
}
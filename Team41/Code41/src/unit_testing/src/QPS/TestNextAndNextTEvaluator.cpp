#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/NextTable.h"
#include "QPS/Evaluator/NextEvaluator.h"
#include "QPS/Evaluator/NextTEvaluator.h"
#include "QPS/ClauseVariable.h"

TEST_CASE("Evaluator: Next and NextT evaluator") {
    string stmt[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    auto* pkbManager = new PKBManager();

    // TODO: Wait for CFG builder to appear

    ClauseVariable integer1(ClauseVariable::variable_type::integer, "1", QueryDeclaration::CONSTANT);
    ClauseVariable integer2(ClauseVariable::variable_type::integer, "2", QueryDeclaration::CONSTANT);
    ClauseVariable integer3(ClauseVariable::variable_type::integer, "3", QueryDeclaration::CONSTANT);
    ClauseVariable integer4(ClauseVariable::variable_type::integer, "4", QueryDeclaration::CONSTANT);
    ClauseVariable integer5(ClauseVariable::variable_type::integer, "5", QueryDeclaration::CONSTANT);
    ClauseVariable integer6(ClauseVariable::variable_type::integer, "6", QueryDeclaration::CONSTANT);

    ClauseVariable synonymS1(ClauseVariable::variable_type::synonym, "s1", QueryDeclaration::ASSIGN);
    ClauseVariable synonymS2(ClauseVariable::variable_type::synonym, "s2", QueryDeclaration::READ);

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", QueryDeclaration::NONE);

    SECTION("Next Evaluator") {
        SECTION("Integer Integer pair") {

        }

        SECTION("Integer Synonym pair") {

        }

        SECTION("Integer Wildcard pair") {

        }

        SECTION("Synonym Integer pair") {

        }

        SECTION("Synonym Synonym pair") {

        }

        SECTION("Synonym Wildcard pair") {

        }

        SECTION("Wildcard Integer pair") {

        }

        SECTION("Wildcard Synonym pair") {

        }

        SECTION("Wildcard Wildcard pair") {

        }
    }

    SECTION("NextT Evaluator") {
        SECTION("Integer Integer pair") {

        }

        SECTION("Integer Synonym pair") {

        }

        SECTION("Integer Wildcard pair") {

        }

        SECTION("Synonym Integer pair") {

        }

        SECTION("Synonym Synonym pair") {

        }

        SECTION("Synonym Wildcard pair") {

        }

        SECTION("Wildcard Integer pair") {

        }

        SECTION("Wildcard Synonym pair") {
        }

        SECTION("Wildcard Wildcard pair") {
        }
    }

    delete pkbManager;
}
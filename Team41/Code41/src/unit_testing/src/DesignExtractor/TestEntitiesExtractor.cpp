#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("DesignExtractor: Register Entities") {
    // Todo: Create ast without Parser
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\tc = 2;\n"
               "\t\twhile (c > 1) {\n"
               "\t\t\tx = y + c;\n"
               "\t\t}\n"
               "\t\tprint a;\n"
               "\t}\n"
               "\tif (f > 1) then {\n"
               "\t\tprint q;\n"
               "\t} else {\n"
               "\t\th = i + j;\n"
               "\t}\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        unordered_set<string> expectedStmt = {"1", "2", "3", "4", "5",
                                              "6", "7", "8", "9", "10"};
        REQUIRE(ee.getNodeToStmtNumMap().size() == expectedStmt.size());
    }

    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }

    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"x", "y", "z", "b", "c", "a",
                                                  "f", "q", "h", "i", "j"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }

    SECTION("Constants") {
        unordered_set<string> expectedConstVals = {"1", "2"};
        REQUIRE(ee.getConstSet() == expectedConstVals);
    }
}

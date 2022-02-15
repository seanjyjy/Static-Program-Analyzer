#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("DesignExtractor: Register Entities") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
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
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h",
                                                  "i", "j", "k", "m", "n", "p", "q", "r", "s", "t"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }

    SECTION("Constants") {
        unordered_set<string> expectedConstVals = {"1", "2", "6", "9"};
        REQUIRE(ee.getConstSet() == expectedConstVals);
    }
}

#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("EntitiesExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 11);
    }
    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }
    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "t", "u", "v", "w", "x", "y", "z"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }
    SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "3", "4", "5", "6", "7", "8"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 19);
    }
    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }
    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "x", "xx", "y", "yy", "z", "zz",
                                                  "v", "vv", "w", "ww", "r", "rr", "s", "ss", "t", "tt", "u", "uu"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }
    SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "11", "2", "3", "4", "5", "6", "7", "8"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 10);
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
        unordered_set<string> expectedConsts = {"1", "2", "6", "9"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

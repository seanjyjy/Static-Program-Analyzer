#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("EntitiesExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while-xml.txt")).build();
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
}

TEST_CASE("EntitiesExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if-xml.txt")).build();
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
}

TEST_CASE("EntitiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested-xml.txt")).build();
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
}

TEST_CASE("EntitiesExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 18);
    }
    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3iif"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }
    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"if", "else", "call", "read", "print", "while", "abe", "x", "y",
                                                  "read", "print", "then", "cream", "dollars"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }
    SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "10", "2", "3", "100", "20", "30", "4", "0"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
}

TEST_CASE("EntitiesExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 13);
    }
    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3iwl"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }
    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"procedure", "a", "b", "c", "if", "else", "then", "print", "and",
                                                  "two", "read", "bob", "builder", "try", "catch", "reader", "while", "x"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }
    SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "98", "20", "0"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
}

TEST_CASE("EntitiesExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 22);
    }
    SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3wim"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }
    SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "x", "x1", "x2", "x3", "x4"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }
    SECTION("Constants") {
        unordered_set<string> expectedConsts = {"10", "20", "2", "0", "4", "3", "1", "30"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
}

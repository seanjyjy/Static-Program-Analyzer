#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl, multi-procedures
 * multiproc1
 */

TEST_CASE("EntitiesExtractor: Assign") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("assign.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 8);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
                                                  "m", "n", "p", "q", "r", "s", "t", "u"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "3", "4", "5", "6", "7", "8"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: Read") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("read.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 3);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "bcd", "e1fg5"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        REQUIRE(ee.getConstSet().empty());
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: Print") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("print.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 3);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "bcd", "e1fg5"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        REQUIRE(ee.getConstSet().empty());
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: While") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("while.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 11);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "t", "u", "v", "w", "x", "y",
                                                  "z"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "3", "4", "5", "6", "7", "8"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: If") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("if.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 19);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "x", "xx", "y", "yy",
                                                  "z", "zz",
                                                  "v", "vv", "w", "ww", "r", "rr", "s", "ss", "t", "tt", "u", "uu"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "11", "2", "3", "4", "5", "6", "7", "8"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: Non-Nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("non_nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 10);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "m", "n"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "3", "4"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 10);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"main"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "e", "f", "g", "h",
                                                  "i", "j", "k", "m", "n", "p", "q", "r", "s", "t"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "6", "9"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: n3iif") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 18);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3iif"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"if", "else", "call", "read", "print", "while", "abe", "x", "y",
                                                  "read", "print", "then", "cream", "dollars"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "10", "2", "3", "100", "20", "30", "4", "0"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 13);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3iwl"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"procedure", "a", "b", "c", "if", "else", "then", "print", "and",
                                                  "two", "read", "bob", "builder", "try", "catch", "reader", "while",
                                                  "x"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"1", "2", "98", "20", "0"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: n3wim") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 22);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3wim"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"a", "b", "c", "d", "x", "x1", "x2", "x3", "x4"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"10", "20", "2", "0", "4", "3", "1", "30"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 9);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n3wwl"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"while", "if", "then", "e2", "a", "b", "c", "else"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {"0", "120348725108321", "3", "4", "1", "10", "2"};
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: multi-procedures") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("multi-procedures.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 6);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"procedure", "read", "print", "call", "while", "if"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {"procedure", "read", "print", "call", "while", "if"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        REQUIRE(ee.getConstSet().empty());
    }
    delete ast;
}

TEST_CASE("EntitiesExtractor: multiproc1") {
    // multiproc/multiproc1-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc1.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();

    SECTION("Statement Nums") {
        REQUIRE(ee.getNodeToStmtNumMap().size() == 23);
    }SECTION("Procedures") {
        unordered_set<string> expectedProcedures = {"n1iif", "n0f", "n0m", "n0l"};
        REQUIRE(ee.getProcSet() == expectedProcedures);
    }SECTION("Variable") {
        unordered_set<string> expectedVarNames = {
                "the3dWORLD", "my2dLAIFU", "TooMuchIntoThings", "er", "Iamn0taWeeb", "isThisJustFantasy",
                "J0000000000000", "A0136999B", "a", "c", "d", "h3ll0", "trader", "buyer", "temptation", "a1", "b1",
                "abc", "xyz", "g00dbye", "yourfather", "permanent", "marker", "a2", "b2", "def", "pqr", "s33y0u"};
        REQUIRE(ee.getVarSet() == expectedVarNames);
    }SECTION("Constants") {
        unordered_set<string> expectedConsts = {
                "12304982739041283675213", "7345612390847123490128", "0", "10", "0", "0"
        };
        REQUIRE(ee.getConstSet() == expectedConsts);
    }
    delete ast;
}

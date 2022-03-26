#include "catch.hpp"
#include "SimpleParser/Parser.h"
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "TestOptimizerUtils.h"

#include <string>

using namespace std;

/*
Test suite:
 - single/multiple clauses
 - classes of clauses: suchthat, with, pattern
 - for each class of clauses, type of clause:
    - suchthat: Follows, Follows*, Parent, Parent*, UsesS, ModifiesP, Calls, Calls*, Next, Next*, Affects, Affects*
    - with: stmt, read, print, call, while, if, assign, variable, constant, procedure
    - pattern: assign, while, if
 */
TEST_CASE("Query Optimizer: integration test1") {
    string simple = "procedure Example {\n"
                    "  x = 2;\n"
                    "  z = 3;\n"
                    "  i = 5;\n"
                    "  while (i!=0) {\n"
                    "    x = x - 1;\n"
                    "    if (x==1) then {\n"
                    "      z = x + 1; }\n"
                    "    else {\n"
                    "      y = z + x; }\n"
                    "    z = z + x + i;\n"
                    "    call q;\n"
                    "    i = i - 1; }\n"
                    "  call p; }\n"
                    "procedure p {\n"
                    "  x = 2; }\n"
                    "procedure q {\n"
                    "  x = 2; }";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, \"i\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    SECTION("Query 2") {
        string query = "assign a;\n"
                       "Select a pattern a(\"z\", _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    SECTION("Query 3") {
        string query = "variable v; procedure p;\n"
                       "Select p such that  Modifies (p, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    delete ast;
}

TEST_CASE("Query Optimizer: integration test2") {
    string simple = "procedure p {\n"
                    "  if (x<0) then {\n"
                    "    while (i>0) {\n"
                    "      x = z * 3 + 2 * y;\n"
                    "      call q;\n"
                    "      i = i - 1; }\n"
                    "    x = x + 1;\n"
                    "    z = x + z; }\n"
                    "  else {\n"
                    "    z = 1; }\n"
                    "  z = z + x + i; }\n"
                    "procedure q {\n"
                    "  x = 2; }";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        // answer pql
        string query = "variable v;\n"
                     "Select v such that Uses(\"p\", v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    delete ast;
}

TEST_CASE("Query Optimizer: integration test3") {
    string simple = "procedure main {\n"
                    "    flag = 0;\n"
                    "    call computeCentroid;\n"
                    "    call printResults;\n"
                    "}\n"
                    "procedure readPoint {\n"
                    "    read x;\n"
                    "    read y;\n"
                    "}\n"
                    "procedure printResults {\n"
                    "    print flag;\n"
                    "    print cenX;\n"
                    "    print cenY;\n"
                    "    print normSq;\n"
                    "}\n"
                    "procedure computeCentroid {\n"
                    "  count = 0;\n"
                    "  cenX = 0;\n"
                    "  cenY = 0;\n"
                    "  call readPoint;\n"
                    "  while ((x != 0) && (y != 0)) {\n"
                    "      count = count + 1;\n"
                    "      cenX = cenX + x;\n"
                    "      cenY = cenY + y;\n"
                    "      call readPoint;\n"
                    "    }\n"
                    "  if (count == 0) then {\n"
                    "      flag = 1;\n"
                    "    } else {\n"
                    "      cenX = cenX / count;\n"
                    "      cenY = cenY / count;\n"
                    "      x = 1;\n"
                    "    }\n"
                    "  normSq = cenX * cenX + cenY * cenY;\n"
                    "}";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        string query = "variable v;\n"
                       "Select v such that Modifies (15, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    SECTION("Query 2") {
        string query = "variable v;\n"
                       "Select v such that Uses (24, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    SECTION("Query 3") {
        string query = "variable v; procedure p;\n"
                       "Select p such that  Modifies (p, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

    delete ast;
}

TEST_CASE("Query Optimizer: autotester 00_assignment") {
    string simple = "procedure Peter {\n"
                    "    print present;\n"
                    "    future = 8 * present;\n"
                    "    call Stephen;\n"
                    "    y = present - 4;\n"
                    "    z = future + 2;\n"
                    "    while (present < 0) {\n"
                    "        x = z - present;\n"
                    "        if (present > x) then {\n"
                    "            present = z - 8 - (x * y) * z;\n"
                    "          x = x - 1; }\n"
                    "          else {\n"
                    "          y = present * y * z;\n"
                    "          call Stephen; }}\n"
                    "    future = x * y * z * present * future;\n"
                    "    print future; }\n"
                    "\n"
                    "procedure Stephen {\n"
                    "    if (future > y) then {\n"
                    "\t        present = z - 8 * present; }\n"
                    "       else {\n"
                    "\t        future = 4 + present * future; }\n"
                    "     call Wong; }\n"
                    "\n"
                    "procedure Wong {\n"
                    "    read x;\n"
                    "    read y; }";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("1 - Select all constants") {
        string query = "constant c;\n"
                       "Select c";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("2 - Test Follows") {
        string query = "stmt s;\n"
                       "Select s such that Follows (8, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("3 - Test Follows*") {
        string query = "stmt s;\n"
                       "Select s such that Follows* (s, 13)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("4 - Test Parent") {
        string query = "stmt s;\n"
                       "Select s such that Parent (s, 12)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("5 - Test Parent*") {
        string query = "assign a;\n"
                       "Select a such that Parent*(_, a)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("6 - Test Modifies") {
        string query = "variable v;\n"
                       "Select v such that Modifies (8, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("7 - Test Uses") {
        string query = "variable v;\n"
                       "Select v such that Uses(3, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("8 - Test SubPattern") {
        string query = "assign a;\n"
                       "Select a pattern a (_, _\"present * future\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("9 - Test SubPattern2") {
        string query = "assign a;\n"
                       "Select a pattern a (_, _\"x*y*z\t\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("10 - Test SubPattern3") {
        string query = "assign a; variable v;\n"
                       "Select v pattern a (v, _\"y\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("11 - Test Modifies") {
        string query = "read re;\n"
                       "Select re such that Modifies (12, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("12 - Test Uses and SubPattern") {
        string query = "assign a; variable v;\n"
                       "Select a such that Uses (a, v) pattern a (v, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("13 - Test Uses and SubPattern2") {
        string query = "assign a; variable v, v1;\n"
                       "Select a such that Uses (a, v) pattern a (v1, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("14 - Test Uses and SubPattern3") {
        string query = "assign a, a1; variable v;\n"
                       "Select v such that Uses (a, v) pattern a1 (v, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("15 - Test Uses and SubPattern4") {
        string query = "assign a, a1; variable v, v1;\n"
                       "Select v such that Uses (a, v) pattern a1 (v1, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
}

TEST_CASE("Query Optimizer: autotester next_cache_queries") {
    string simple = "procedure NextTest {\n"
                    "    x = 1;\n"
                    "    x = 2;\n"
                    "    if (z == 2) then {\n"
                    "        while (i != 0) {\n"
                    "            x = x - 1;\n"
                    "            print t;\n"
                    "            if (x == 1) then {\n"
                    "                while (y == 0) {\n"
                    "                    read y;\n"
                    "                    while (g == 0) {\n"
                    "                        x = x + 1;\n"
                    "                    }\n"
                    "                    call x;\n"
                    "                }\n"
                    "            } else {\n"
                    "               print y;\n"
                    "               call x;\n"
                    "               print t;\n"
                    "               while (x == y) {\n"
                    "                   t = t + 1;\n"
                    "                   read t;\n"
                    "                   call t;\n"
                    "               }\n"
                    "            }\n"
                    "            x = x + 1;\n"
                    "        }\n"
                    "    } else {\n"
                    "        x = x + 2;\n"
                    "        while (x == z) {\n"
                    "            read z;\n"
                    "            read y;\n"
                    "        }\n"
                    "    }\n"
                    "    x = x + 1;\n"
                    "    y = y + 2;\n"
                    "}\n"
                    "\n"
                    "procedure x {\n"
                    "    x = x + 1;\n"
                    "}\n"
                    "\n"
                    "procedure t {\n"
                    "    t = t + 1;\n"
                    "}";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("1 - Next(int, int) - Next(int, int) - same") {
        string query = "Select BOOLEAN such that Next(4, 25) and Next(4, 25)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("2 - Next(int, int) - Next(int, int) - both true") {
        string query = "Select BOOLEAN such that Next(4, 5) and Next(4, 25)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("3 - Next(int, a) - Next*(int, a)") {
        string query = "assign a;\n"
                       "Select a such that Next(4, a) and Next*(3, a)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("4 - Next(s, s2) - Next*(s, s2)") {
        string query = "stmt s, s2;\n"
                       "Select <s, s2> such that Next(s, s2) and Next*(s, s2)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("5 - Next(s1, s2) - Next(s2, s3)") {
        string query = "stmt s1, s2, s3;\n"
                       "Select <s1, s3> such that Next(s1, s2) and Next(s2, s3)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("6 - Next(s, s2) - Next*(s2, s)") {
        string query = "stmt s, s2;\n"
                       "Select <s, s2> such that Next(s, s2) and Next*(s2, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("7 - Next*(s1, s2) - Next*(s2, s3)") {
        string query = "stmt s1, s2, s3;\n"
                       "Select <s1, s3> such that Next*(s1, s2) and Next*(s2, s3)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("8 - Next*(s1, s2) - Next*(s2, s3) - Next*(s2, s1)") {
        string query = "stmt s1, s2, s3;\n"
                       "Select <s1, s3> such that Next*(s1, s2) and Next*(s2, s3) and Next*(s2, s1)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("9 - Next*(a1, a2) - Next*(a2, a3) - Next*(a3, a4)") {
        string query = "assign s1, s2, s3, s4;\n"
                       "Select <s1, s3> such that Next*(s1, s2) and Next*(s2, s3) and Next*(s3, s4)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("10 - Next*(1, s) - Next*(1, 26)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(1, s) and Next*(1, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("11 - Next*(1, s) - Next*(2, s)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(1, s) and Next*(2, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("12 - Next*(1, s) - Next*(2, s) - Next*(3, s)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(1, s) and Next*(2, s) and Next*(3, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("13 - Next*(2, s) - Next*(1, s)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(2, s) and Next*(1, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("14 - Next*(3, s) - Next*(2, s) - Next*(1, s)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(3, s) and Next*(2, s) and Next*(1, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("15 - Next*(s, 19) - Next*(s, 18)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(s, 19) and Next*(s, 18)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("16 - Next*(s, 19) - Next*(s, 18) - Next*(s, 17)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(s, 19) and Next*(s, 18) and Next*(s, 17)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("17 - Next*(s, 18) - Next*(s, 19)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(s, 18) and Next*(s, 19)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("18 - Next*(s, 17) - Next*(s, 18) - Next*(s, 19)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(s, 17) and Next*(s, 18) and Next*(s, 19)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("19 - Next*(s, 26) - Next*(1, 26)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(s, 26) and Next*(1, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("20 - Next*(1, s) - Next*(s, 26)") {
        string query = "stmt s;\n"
                       "Select s such that Next*(1, s) and Next*(s, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("21 - Next*(1, s) - Next*(s, 26) - Next*(1, 26)") {
        string query = "stmt s, s1, s2;\n"
                       "Select s such that Next*(1, s) and Next*(s, 26) and Next*(1, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("22 - Next*(s1, s2) - Next*(1, s)") {
        string query = "stmt s, s1, s2;\n"
                       "Select s such that Next*(s1, s2) and Next*(1, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("23 - Next*(s1, s2) - Next*(s, 26)") {
        string query = "stmt s, s1, s2;\n"
                       "Select s such that Next*(s1, s2) and Next*(s, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("24 - Next*(s1, s2) - Next*(1, 26)") {
        string query = "stmt s, s1, s2;\n"
                       "Select BOOLEAN such that Next*(s1, s2) and Next*(1, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("25 - Next*(s1, s2) - Next*(1, s) - Next*(s, 26)") {
        string query = "stmt s, s1, s2;\n"
                       "Select s such that Next*(s1, s2) and Next*(1, s) and Next*(s, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("26 - Next*(s1, s2) - Next*(1, s) - Next*(s, 26) - Next*(1, 26)") {
        string query = "stmt s, s1, s2;\n"
                       "Select s such that Next*(s1, s2) and Next*(1, s) and Next*(s, 26) and Next*(1, 26)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
}


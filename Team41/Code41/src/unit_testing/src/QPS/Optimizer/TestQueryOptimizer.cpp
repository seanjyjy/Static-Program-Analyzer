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

TEST_CASE("Query Optimizer: autotester 01_uses_modify") {
    string simple = "procedure procedure {\n"
                    "  x411 = 2;\n"
                    "  y132 = 2 + 2;\n"
                    "  x1c2v3b4 = 2 / 2;\n"
                    "\n"
                    "  x = y132 * 2;\n"
                    "  x = 3 % x;  x = 2 * 2 + 3 % 1 - x411 / 2;\n"
                    "  var = 2;\n"
                    "  foo = 4;\n"
                    "  bar = foo - 4 + 3 * 2;\n"
                    "  X=1;\n"
                    "    read bar ;\n"
                    "   \tbar= foo   ;\n"
                    "   if (a <\n"
                    "   bar) then{\n"
                    "      while (bar > temp) {\n"
                    "           oSCar  = 1 * bar + tmp;\n"
                    "           while (!(tmp / tmp == bar * bar)) {\n"
                    "             oSCar = X - (bar + foo *\n"
                    "             chArlie); }}\n"
                    "   \twhile ((!(x!=1)) && (!(x == 1))) {\n"
                    "           x = x + 1;\n"
                    "           if (foo==0) then {\n"
                    "             while (bar== 3){\n"
                    "               print A1pH3;\n"
                    "               b = 0;\n"
                    "               c = x411    + z + A1pH3; }}\n"
                    "           else {\n"
                    "               while (c>1) {\n"
                    "                              c = c -1;}\n"
                    "               x = x+ 1; }}}\n"
                    "   else{\n"
                    "   \ta= 2;}\n"
                    "  while (foo < bar) {\n"
                    "    if (var > 0) then {\n"
                    "      var = var + 22222222222222222222222222222;\n"
                    "      bar = bar + 11111111111111111111111111111;\n"
                    "      var = var - 22222222222222222222222222222;\n"
                    "    } else {\n"
                    "      foo = foo + 1;\n"
                    "      var = var - 1;\n"
                    "      while = 8;\n"
                    "      while (while > 1) {\n"
                    "        while = 3 * 2 + while / 2 - 6;\n"
                    "        read read;\n"
                    "        read print;\n"
                    "        then\n"
                    "        =\n"
                    "        read\n"
                    "        ;\n"
                    "        else =\n"
                    "        print;\n"
                    "        if (while != 0) then {\n"
                    "          temp = while * 2 - 2;\n"
                    "          read while;\n"
                    "          while = foo - bar * var + 3 * 2;\n"
                    "          print while;\n"
                    "          while = (temp + 2) / 2;\n"
                    "        } else {\n"
                    "          read temp;\n"
                    "          print temp;\n"
                    "        }\n"
                    "        print read;\n"
                    "        print print;\n"
                    "      }\n"
                    "    }\n"
                    "  }\n"
                    "  print var;\n"
                    "  print foo;\n"
                    "  print bar;\n"
                    "}";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("1 - Select all statement") {
        string query = "stmt s;\n"
                       "Select s";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("2 - Select all variable") {
        string query = "variable v;\n"
                       "Select v";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("3 - Select all constant") {
        string query = "constant c;\n"
                       "Select c";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("4 - Select all print") {
        string query = "print pn;\n"
                       "Select pn";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("5 - Select all read") {
        string query = "read re;\n"
                       "Select re";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("6 - Select all while") {
        string query = "while w;\n"
                       "Select w";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("7 - Select all if") {
        string query = "if ifs;\n"
                       "Select ifs";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("8 - Uses with invalid wildcard") {
        string query = "variable v;\n"
                       "Select BOOLEAN such that Uses(_, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("9 - Uses with statement and wildcard") {
        string query = "stmt s;\n"
                       "Select s such that Uses(s, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("10 - Uses with assign and ident variable") {
        string query = "assign a;\n"
                       "Select a such that Uses(a, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("11 - Uses with assign and synonym variable") {
        string query = "assign a; variable v;\n"
                       "Select a such that Uses(a, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("12 - Uses with ident assign and synonym variable") {
        string query = "variable v;\n"
                       "Select v such that Uses(14, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("13 - Uses with print and ident variable") {
        string query = "print pn; variable v;\n"
                       "Select v such that Uses(pn, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("14 - Uses with if and ident variable") {
        string query = "if ifs;\n"
                       "Select ifs such that Uses(ifs, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("15 - Uses with if and synonym variable") {
        string query = "if ifs; variable v;\n"
                       "Select v such that Uses(ifs, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("16 - Uses with while and ident variable") {
        string query = "while w;\n"
                       "Select w such that Uses(w, \"X\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("17 - Uses with while and synonym variable") {
        string query = "while w; variable v;\n"
                       "Select v such that Uses(w, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("18 - Uses with false clause") {
        string query = "stmt s;\n"
                       "Select s such that Uses(6, \"y132\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("19 - Uses with true clause") {
        string query = "Select v such that Uses(6, \"x411\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("20 - Uses none") {
        string query = "stmt s;\n"
                       "Select s such that Uses(s, \"Z\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("21 - Modifies with invalid wildcard") {
        string query = "variable v;\n"
                       "Select BOOLEAN such that Modifies(_, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("22 - Modifies with statement and wildcard") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("23 - Modifies with assign and ident variable") {
        string query = "assign a;\n"
                       "Select a such that Modifies(a, \"x1c2v3b4\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("24 - Modifies with assign and synonym variable") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(a, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("25 - Modifies with ident assign and synonym variable") {
        string query = "variable v;\n"
                       "Select v such that Modifies(1, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("26 - Modifies with read and synonym variable") {
        string query = "read r; variable v;\n"
                       "Select v such that Modifies(r, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("27 - Modifies with if and ident variable") {
        string query = "if ifs;\n"
                       "Select ifs such that Modifies(ifs, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("28 - Modifies with if and ident variable - none") {
        string query = "if ifs;\n"
                       "Select ifs such that Modifies(ifs, \"x411\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("29 - Modifies with if and ident variable") {
        string query = "if ifs; variable v;\n"
                       "Select v such that Modifies(ifs, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("30 - Modifies with while and ident variable") {
        string query = "while w;\n"
                       "Select w such that Modifies(w, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("31 - Modifies with while and synonym variable") {
        string query = "variable v; while w;\n"
                       "Select v such that Modifies(w, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("32 - Modifies none") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, \"xDoNotExist\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("41 - Invalid case - Uses with read") {
        string query = "read r;\n"
                       "Select BOOLEAN such that Uses(r,_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("42 - Invalid case - Modifies with print") {
        string query = "print pn;\n"
                       "Select BOOLEAN such that Modifies(pn,_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("46 - Uses('procedure', _)") {
        string query = "Select BOOLEAN such that Uses(\"procedure\", _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("47 - Uses('procedure', 'x')") {
        string query = R"(Select BOOLEAN such that Uses("procedure", "x"))";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("48 - Uses('procedure', 'x')") {
        string query = "variable v;\n"
                       "Select BOOLEAN such that Uses(\"procedure\", \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("49 - Uses('procedure', proc)") {
        string query = "procedure Uses;\n"
                       "Select BOOLEAN such that Uses(\"procedure\", Uses)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("50 - Uses('procedure', v)") {
        string query = "variable Select;\n"
                       "Select Select such that Uses(\"procedure\", Select)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("51 - Uses('procedure', 1)") {
        string query = "Select BOOLEAN such that Uses(\"procedure\", 1)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("52 - Uses(proc, v) - select proc") {
        string query = "variable Uses; procedure such;\n"
                       "Select such such that Uses(such, Uses)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("53 - Uses(proc, 'chArlie')") {
        string query = "variable Select; procedure proc;\n"
                       "Select proc such that Uses(proc, \"chArlie\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("54 - Uses(proc, v) - select v") {
        string query = "variable variable; procedure procedure;\n"
                       "Select variable such that Uses(procedure, variable)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("55 - Uses(s, 'X')") {
        string query = "stmt stmt;\n"
                       "Select stmt such that Uses(stmt, \"X\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("56 - Modifies('procedure', _)") {
        string query = "Select BOOLEAN such that Modifies(\"procedure\", _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("66 - Modifies(s, v)") {
        string query = "stmt stmt; variable variable;\n"
                       "Select <stmt, variable> such that Modifies(stmt, variable)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("67 - Modifies(proc, v)") {
        string query = "procedure procedure; variable variable;\n"
                       "Select <procedure, variable> such that Modifies(procedure, variable)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("68 - Uses(s, v)") {
        string query = "stmt stmt; variable variable;\n"
                       "Select <stmt, variable> such that Uses(stmt, variable)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("69 - Uses(proc, v)") {
        string query = "procedure procedure; variable variable;\n"
                       "Select <procedure, variable> such that Uses(procedure, variable)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
}

TEST_CASE("Query Optimizer: autotester 05_uses_modify_pattern") {
    string simple = " procedure wH5TH15m0dS0Pa1nFu1 {x = (NevERGONNA12GiveYouUP12-10*(37 - (25 + thisIs100PercentTheStartOfT24134QSDAFAF)) + 50 / 20);print thisIs100PercentTheStartOfT24134QSDAFAF;\n"
                    "    if ((seeThisRub115h == d1Ngd1Ngd1Ng) || ((elephant == FATBOM123BOM) && (d1Ngd1Ngd1Ng != z1b3sw))) then {\n"
                    "        print\n"
                    "        d1Ngd1Ngd1Ng;\n"
                    "        if (\n"
                    "        seeThisRub115h\n"
                    "        ==\n"
                    "        z1b3sw\n"
                    "        )\n"
                    "        then { while ((b == d1Ngd1Ngd1Ng) && (T0ngT0ngQ1ANG == NevERGONNA12GiveYouUP12)) {\n"
                    "x               =                x     ; print i; if\n"
                    "((d1Ngd1Ngd1Ng ==                          NevERGONNA12GiveYouUP12        )                 ||\n"
                    "(g == b))\n"
                    "then {T0ngT0ngQ1ANG=h+y+                 2+((x))*            g         ;               h = (77 -\n"
                    "(g  +\n"
                    "y)) * (g\n"
                    "* (((g + g)\n"
                    "+ (g * y)\n"
                    ")\n"
                    ")\n"
                    ")\n"
                    ";\n"
                    "                } else {h = abcdef;}print l;}if(d1Ngd1Ngd1Ng==b)then{print thisIs100PercentTheStartOfT24134QSDAFAF;while(jb==noChance){cry=cry+1;}\n"
                    "                thisIs100PercentTheStartOfT24134QSDAFAF = b;\n"
                    "} else {\n"
                    "while (zz == tc) {zz=2+zz*h+y;}\n"
                    "print\n"
                    "ab; read\n"
                    "db;\n"
                    "ab=                     db;while\n"
                    "(ab\n"
                    "==\n"
                    "aj) {tj = (abcdef\n"
                    "+ abcdef\n"
                    ") -\n"
                    "abcdef\n"
                    ";\n"
                    "                }\n"
                    "}print\n"
                    "T0ngT0ngQ1ANG;while (ab == ac) {\n"
                    "ab\n"
                    "=\n"
                    "1\n"
                    ";\n"
                    "print\n"
                    "ac\n"
                    ";\n"
                    "}\n"
                    "}\n"
                    "else\n"
                    "{print sadness;while (iAmSad123 != youAreSad321) {weCry =\n"
                    "youCry + weCry\n"
                    ";\n"
                    "print cry;if (youWeep\n"
                    "== iWeep) then {weAllWeep=\n"
                    "1\n"
                    ";print cry;}\n"
                    "else\n"
                    "{print noCry\n"
                    ";theyAllHeartLess = bastards\n"
                    "+ heartless\n"
                    ";\n"
                    "}\n"
                    "}\n"
                    "read weAllCry;\n"
                    "}\n"
                    "} else {while (else ==\n"
                    "sad) {then = lol;print whyAreKeyWordsEvenUsed;}if (youReachedHere == youPsycho) then {enjoyThisReallyFunLongText=kidding+(g+g);okTimeToEndSoon=yesPls+g+g;} else {\n"
                    "PLSEND =\n"
                    "OK\n"
                    ";}while\n"
                    "(neverGonnaLetThisEnd\n"
                    "==\n"
                    "sadness) {print iAmSad123;print youAreSad321;bastards = VERYTRUE;read helloWord;while (youReachedHere == youPsycho){then=youAreSad321;read moreToK1M2S;}}}}";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("149 - Modifies Synonym WildCard Pattern Pair5") {
        string query = "assign a;\n"
                       "Select a such that Modifies(a, _) pattern a(\"ab\", \"1\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("148 - Modifies Synonym WildCard Pattern Pair4") {
        string query = "stmt s; assign a;\n"
                       "Select s such that Modifies(s, _) pattern a(\"okTimeToEndSoon\", \"yesPls + (g + g)\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("147 - Modifies Synonym WildCard Pattern Pair3") {
        string query = "if ifs; assign a;\n"
                       "Select ifs such that Modifies(ifs, _) pattern a(\"enjoyThisReallyFunLongText\", \"(kidding + g) + g\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("146 - Modifies Synonym WildCard Pattern Pair2") {
        string query = "while w; assign a;\n"
                       "Select w such that Modifies(w, _) pattern a(\"theyAllHeartLess\", \"bastards + heartless\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("145 - Modifies Synonym WildCard Pattern Pair1") {
        string query = "read re; assign a;\n"
                       "Select re such that Modifies(re, _) pattern a(\"x\", \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("144 - Modifies Synonym WildCard SubPattern Pair5") {
        string query = "assign a;\n"
                       "Select a such that Modifies(a, _) pattern a(_, _\"g +     g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("143 - Modifies Synonym WildCard SubPattern Pair4") {
        string query = "stmt s; assign a;\n"
                       "Select s such that Modifies(s, _) pattern a(\"x\", _\"y\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("142 - Modifies Synonym WildCard SubPattern Pair3") {
        string query = "if ifs; assign a;\n"
                       "Select ifs such that Modifies(ifs, _) pattern a(\"tj\", _\"abcdef - abcdef\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("141 - Modifies Synonym WildCard SubPattern Pair2") {
        string query = "while w; assign a;\n"
                       "Select w such that Modifies(w, _) pattern a(_, _\"zz * h\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("140 - Modifies Synonym WildCard SubPattern Pair1") {
        string query = "read re; assign a;\n"
                       "Select re such that Modifies(re, _) pattern a(_, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("139 - Modifies Synonym Synonym Pattern Pair5") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(a, v) pattern a(v, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("138 - Modifies Synonym Synonym Pattern Pair4") {
        string query = "stmt s; variable v; assign a;\n"
                       "Select s such that Modifies(s, v) pattern a(\"h\", 77 - g  + y * g * g + g + g * y)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("137 - Modifies Synonym Synonym Pattern Pair3") {
        string query = "if ifs; variable v; assign a;\n"
                       "Select ifs such that Modifies(ifs, v) pattern a(\"x\", \"x + 1\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("136 - Modifies Synonym Synonym Pattern Pair2 - Expected empty") {
        string query = "while w; variable v; assign a;\n"
                       "Select w such that Modifies(w, v) pattern a(bastards, \"VERYTRUE\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("135 - Modifies Synonym Synonym Pattern Pair1") {
        string query = "read re; variable v; assign a;\n"
                       "Select re such that Modifies(re, v) pattern a(\"x\", \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("134 - Modifies Synonym Synonym SubPattern Pair5") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(a, v) pattern a(v, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("133 - Modifies Synonym Synonym SubPattern Pair4") {
        string query = "stmt s; variable v; assign a;\n"
                       "Select s such that Modifies(s, v) pattern a(\"enjoyThisReallyFunLongText\", _\"kidding + g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("132 - Modifies Synonym Synonym SubPattern Pair3") {
        string query = "if ifs; variable v; assign a;\n"
                       "Select ifs such that Modifies(ifs, v) pattern a(v, _\"g + g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("131 - Modifies Synonym Synonym SubPattern Pair2") {
        string query = "while w; variable v; assign a;\n"
                       "Select w such that Modifies(w, v) pattern a(v, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("130 - Modifies Synonym Synonym SubPattern Pair1") {
        string query = "read re; variable v; assign a;\n"
                       "Select re such that Modifies(re, v) pattern a(v, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("129 - Modifies Synonym Identifier Pattern Pair5") {
        string query = "assign a;\n"
                       "Select a such that Modifies(a, \"cry\") pattern a(\"cry\", \"cry + 1\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("128 - Modifies Synonym Identifier Pattern Pair4") {
        string query = "stmt s; assign a;\n"
                       "Select s such that Modifies(s, \"zz\") pattern a(\"enjoyThisReallyFunLongText\", (kidding + g) + g);";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("127 - Modifies Synonym Identifier Pattern Pair3") {
        string query = "if ifs; assign a;\n"
                       "Select ifs such that Modifies(ifs, \"zz\") pattern a(_, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("126 - Modifies Synonym Identifier Pattern Pair2") {
        string query = "while w; assign a;\n"
                       "Select w such that Modifies(w, \"zz\") pattern a(_, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("125 - Modifies Synonym Identifier Pattern Pair1") {
        string query = "read re; assign a;\n"
                       "Select re such that Modifies(re, \"moreToK1M2S\") pattern a(\"okTimeToEndSoon\", \"yesPls + g + g\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("124 - Modifies Synonym Identifier SubPattern Pair5") {
        string query = "assign a;\n"
                       "Select a such that Modifies(a, \"x\") pattern a(\"x\", _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("123 - Modifies Synonym Identifier SubPattern Pair4") {
        string query = "stmt s; assign a;\n"
                       "Select s such that Modifies(s, \"h\") pattern a(_, _\"77 - g  + y\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("122 - Modifies Synonym Identifier SubPattern Pair3") {
        string query = "if ifs; assign a;\n"
                       "Select ifs such that Modifies(ifs, \"weCry\") pattern a(_, _\"1\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("121 - Modifies Synonym Identifier SubPattern Pair2") {
        string query = "while w; assign a;\n"
                       "Select w such that Modifies(w, \"T0ngT0ngQ1ANG\") pattern a(_, _\" h + y \"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("120 - Modifies Synonym Identifier SubPattern Pair1") {
        string query = "read re; assign a;\n"
                       "Select re such that Modifies(re, \"weAllCry\") pattern a(_, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("119 - Modifies Integer WildCard Pattern Pair2") {
        string query = "assign a;\n"
                       "Select a such that Modifies(31, _) pattern a(\"x\", \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("118 - Modifies Integer WildCard Pattern Pair1") {
        string query = "assign a;\n"
                       "Select a such that Modifies(31, _) pattern a(_, \"heartless\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("117 - Modifies Integer WildCard SubPattern Pair2") {
        string query = "assign a; variable v;\n"
                       "Select v such that Modifies(21, _) pattern a(_, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("116 - Modifies Integer WildCard SubPattern Pair1") {
        string query = "assign a;\n"
                       "Select a such that Modifies(1, _) pattern a(\"x\", _\"x + 1\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("115 - Modifies Integer Synonym Pattern Pair2") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(47, v) pattern a(\"bastards\", \"VERYTRUE\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("114 - Modifies Integer Synonym Pattern Pair1") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(3, v) pattern a(_, \"x\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("113 - Modifies Integer Synonym SubPattern Pair4") {
        string query = "assign a; variable v;\n"
                       "Select v such that Modifies(47, v) pattern a(_, _\"kidding + g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("112 - Modifies Integer Synonym SubPattern Pair3") {
        string query = "assign a; variable v;\n"
                       "Select v such that Modifies(47, v) pattern a(_, _\"yesPls + g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("111 - Modifies Integer Synonym SubPattern Pair2") {
        string query = "assign a; variable v;\n"
                       "Select a such that Modifies(6, v) pattern a(v, _\"g\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }

}

TEST_CASE("Query Optimizer: autotester 09_assignment2") {
    string simple = "procedure Monk {\n"
                    "      a = 4;\n"
                    "      read t;\n"
                    "      c = a * b + 3 * a;\n"
                    "      call Pig;\n"
                    "      y = a + 2 * c;\n"
                    "      while (d != 1)  {\n"
                    "          d = a + y;\n"
                    "          while (y < c) {\n"
                    "              x = b + 5 * y;\n"
                    "            t = 1;\n"
                    "            call Monkey; }\n"
                    "        v = d + 25; }\n"
                    "    x = 2 * t + v;\n"
                    "    if (a > 0) then {\n"
                    "        x = x * a; }\n"
                    "      else {\n"
                    "       x = x + a; }\n"
                    "    y = x + 2;\n"
                    "    call Kappa; }\n"
                    "procedure Pig {\n"
                    "    while (c > 0) {\n"
                    "        if (b > 0) then {\n"
                    "            d = v + a; }\n"
                    "           else {\n"
                    "            a = x * y + v * y + d; } } }\n"
                    "procedure Monkey {\n"
                    "    t = d + y;\n"
                    "    if (t > 0) then {\n"
                    "        while (x > 0) {\n"
                    "            call Pig;\n"
                    "            d = d + a; } }\n"
                    "       else {\n"
                    "            t = d + 21 + t; } }\n"
                    "procedure Kappa {\n"
                    "    print x;\n"
                    "    call Monkey;\n"
                    "    }";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("1 - cl with attr ref") {
        string query = "call cl;\n"
                       "Select <cl.procName>";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("2 - calls p") {
        string query = "procedure p;\n"
                       "Select p such that Calls (p, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("3 - next (a, w)") {
        string query = "assign a; while w;\n"
                       "Select a such that Next (a, w)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("4 - Affects (13, 17)") {
        string query = "Select BOOLEAN such that Affects(13, 17)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("5 - Affects (a, 13)") {
        string query = "assign a;\n"
                       "Select a such that Affects(a, 13)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("6 - pattern with") {
        string query = "while w; if ifs;\n"
                       "Select <w, ifs> pattern w (\"c\", _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("7 - with stmt# - constant value") {
        string query = "constant c; assign a;\n"
                       "Select c with a.stmt# = c.value";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("8 - incorrect pattern and") {
        string query = "assign a, a1;\n"
                       "Select a pattern a (\"x\", _) and pattern a1 (_, _\"x\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("9 - Next cache usage") {
        string query = "stmt s; call cl;\n"
                       "Select s such that Next* (s, s) such that Next* (cl, s)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("10 - Affects with If") {
        string query = "assign a; if ifs;\n"
                       "Select a such that Affects*(a, a) and Parent* (ifs, a)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("11 - Tuple with Follow and Pattern While") {
        string query = "assign a; variable v; while w;\n"
                       "Select <a, v> such that Follows (w, a) pattern w (v, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("12 - pattern ifs and modifies and uses") {
        string query = "procedure p; if ifs; variable v;\n"
                       "Select p pattern ifs (v, _, _) such that Modifies (p, v) and Uses (p, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("13 - Affects pattern with attr ref") {
        string query = "assign a, a1; variable v; read rd;\n"
                       "Select a1 such that Affects* (a, a1) pattern a (v, _) with v.varName = rd.varName";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
}

TEST_CASE("Query Optimizer: autotester 12_next_cache_queries") {
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

TEST_CASE("Query Optimizer: autotester 13_sample") {
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
                    "\n"
                    "procedure p {\n"
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
                    "\n"
                    "procedure q {\n"
                    "  if (x==1) then {\n"
                    "    z = x + 1; }\n"
                    "  else {\n"
                    "    x = z + x; } }";

    // parse program
    Parser p;
    TNode *ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("1") {
        string query = "while w;\n"
                       "Select w such that Parent(w, 7)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("2") {
        string query = "if ifs;\n"
                       "Select ifs such that Follows(5, ifs)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("3") {
        string query = "assign a;\n"
                       "Select a such that Parent*(4, a)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("4") {
        string query = "call c;\n"
                       "Select c such that Follows*(1, c)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("5") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, \"i\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("6") {
        string query = "variable v;\n"
                       "Select v such that Uses(\"p\", v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("7") {
        string query = "procedure p;\n"
                       "Select p such that Calls(p, \"q\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("8") {
        string query = "procedure p;\n"
                       "Select p such that Calls*(\"Example\", p)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("9") {
        string query = "stmt n;\n"
                       "Select n such that Next(4, n)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("10") {
        string query = "stmt n;\n"
                       "Select n such that Next*(n, 4)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("11") {
        string query = "assign a;\n"
                       "Select a such that Affects(a, 9)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("12") {
        string query = "assign a;\n"
                       "Select a such that Affects*(11, a)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("13") {
        string query = "assign a;\n"
                       "Select a pattern a(\"z\", _\"x+i\")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("14") {
        string query = "assign a;\n"
                       "Select a pattern a(_, _\"x + 1\"_)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("15") {
        string query = "assign a;\n"
                       "Select a pattern a(_, \" 3  +  2 \")";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("16") {
        string query = "while w; variable v;\n"
                       "Select w such that Modifies(w, v) with v.varName = \"x\"";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("17") {
        string query = "if ifs; variable v;\n"
                       "Select v such that Uses(ifs, v) with ifs.stmt# = 22";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("18") {
        string query = "procedure p, q;\n"
                       "Select BOOLEAN such that Calls(p, q) with q.procName = \"p\" and p.procName = \"Example\"";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("19") {
        string query = "if ifs; assign a1, a2; variable v1, v2;\n"
                       "Select ifs such that Follows*(a1, ifs) and Follows*(ifs, a2) and Modifies(ifs, v1) and Uses(ifs, v2) with v1.varName = v2.varName";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("20") {
        string query = "stmt n; stmt s;\n"
                       "Select s such that Next*(16, n) and Parent*(s, n)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("21") {
        string query = "stmt n; assign a;\n"
                       "Select a such that Affects*(a, n) and Next*(13, n)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("22") {
        string query = "procedure p, q; variable v;\n"
                       "Select <p, q, v> such that Modifies(p, v) and Calls(p, q)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("23") {
        string query = "call c; assign a1, a2;\n"
                       "Select BOOLEAN such that Follows*(_, c) and Affects(a1, a2) and Uses(a2, _)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("24") {
        string query = "assign a1, a2; variable v;\n"
                       "Select v pattern a1(v, _) such that Affects*(a1, a2) and Uses(a2, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
    SECTION("25") {
        string query = "stmt n1, n2; variable v; call c;\n"
                       "Select c such that Next*(n1, n2) and Modifies(n1, v) and Uses(n2, v) and Modifies(c, v)";
        TestOptimizerUtils::ensureOQOIsCorrect(query, pkbManager);
    }
}

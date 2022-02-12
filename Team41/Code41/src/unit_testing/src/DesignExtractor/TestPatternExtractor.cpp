#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/PatternExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("PatternExtractor: Non-nested") {
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\ta = z + y - 2 * 3 / x - 1;\n"
               "\t\tb = 2;\n"
               "\t}\n"
               "\tif (f > 1) then {\n"
               "\t\tb = 3;\n"
               "\t} else {\n"
               "\t\tprint a;"
               "\t\tread b;"
               "\t}\n"
               "\tprint k;"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("AssignPattern") {
        unordered_map<string, pair<string, string>> expectedPattern = {
                {"2", {"y", p.parseExpr(("z + 1"))->toStringRecursive()}},
                {"4", {"a", p.parseExpr(("z + y - 2 * 3 / x - 1"))->toStringRecursive()}},
                {"5", {"b", p.parseExpr(("2"))->toStringRecursive()}},
                {"7", {"b", p.parseExpr(("3"))->toStringRecursive()}}
        };
        unordered_map<string, pair<string, string>> parsedResult;
        for(auto [stmt, pair] : pe.getAssignPatternMap()) {
            parsedResult.insert({stmt, {pair.first, pair.second->toStringRecursive()}});
        }
        REQUIRE(parsedResult == expectedPattern);
    }
}


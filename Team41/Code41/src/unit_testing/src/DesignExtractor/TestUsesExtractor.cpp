#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/UsesExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("UsesExtractor: Non-nested") {
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = x + z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\ta = 2;\n"
               "\t}\n"
               "\tif (f > 1) then {\n"
               "\t\td = 3;\n"
               "\t} else {\n"
               "\t\tprint k;"
               "\t}\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    SECTION("Procedure Uses") {
        unordered_map<string, unordered_set<string>> expectedProcUses = {
                {"main", {"x", "z", "b", "f", "k"}}
        };
        REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    }

    SECTION("Statement Uses") {
        unordered_map<string, unordered_set<string>> expectedStmtUses = {
                {"2", {"x", "z"}}, {"3", {"b"}},
                {"5", {"f", "k"}}, {"7", {"k"}}
        };
        REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    }
}

TEST_CASE("UsesExtractor: Nested") {
    string s = "procedure main {\n"
               "\twhile ((b != 1) && (d < 2)) {\n"
               "\t\tread x;\n"
               "\t\tif ((f > 1) || (g >= 5)) then {\n"
               "\t\t\ty = z + 1;\n"
               "\t\t} else {\n"
               "\t\t\ta = k + 2;\n"
               "\t\t}\n"
               "\t}\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    SECTION("Procedure Uses") {
        unordered_map<string, unordered_set<string>> expectedProcUses = {
                {"main", {"b", "d", "f", "g", "z", "k"}}
        };
        REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    }

    SECTION("Statement Uses") {
        unordered_map<string, unordered_set<string>> expectedStmtUses = {
                {"1", {"b", "d", "f", "g", "z", "k"}},
                {"3", {"f", "g", "z", "k"}},
                {"4", {"z"}}, {"5", {"k"}}
        };
        REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    }
}


#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ParentExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("ParentExtractor: Non-nested") {
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\ta = 2;\n"
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
    Parser p = Parser{s};
    TNode* ast = p.parse();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("Parent") {
        unordered_map<string, list<string>> expectedParent = {
                {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
        };
        REQUIRE(pe.getParentMap() == expectedParent);
    }

    SECTION("ParentT") {
        unordered_map<string, list<string>> expectedParentT = {
                {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
        };
        REQUIRE(pe.getParentTMap() == expectedParentT);
    }
}

TEST_CASE("ParentExtractor: Nested") {
    string s = "procedure main {\n"
               "\twhile (b != 1) {\n"
               "\t\tread x;\n"
               "\t\tif (f > 1) then {\n"
               "\t\t\ty = z + 1;\n"
               "\t\t\ta = b + 2;\n"
               "\t\t} else {\n"
               "\t\t\ta = b + 2;\n"
               "\t\t\twhile (b != 1) {\n"
               "\t\t\t\ty = z + 1;\n"
               "\t\t\t\ta = b + 2;\n"
               "\t\t\t\tb = c + 2;\n"
               "\t\t\t}\n"
               "\t\t}\n"
               "\t}\n"
               "}";
    Parser p = Parser{s};
    TNode* ast = p.parse();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("Parent") {
        unordered_map<string, list<string>> expectedParent = {
                {"1", {"2", "3"}}, {"3", {"4", "5", "6", "7"}}, {"7", {"8", "9", "10"}}
        };
        REQUIRE(pe.getParentMap() == expectedParent);
    }

    SECTION("ParentT") {
        unordered_map<string, list<string>> expectedParentT = {
                {"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10"}},
                {"3", {"4", "5", "6", "7", "8", "9", "10"}}, {"7", {"8", "9", "10"}}
        };
        REQUIRE(pe.getParentTMap() == expectedParentT);
    }
}


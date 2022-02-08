#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("ModifiesExtractor: Non-nested") {
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\ta = 2;\n"
               "\t}\n"
               "\tif (f > 1) then {\n"
               "\t\tb = 3;\n"
               "\t} else {\n"
               "\t\tprint a;"
               "\t}\n"
               "\tprint k;"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

    SECTION("Procedure Modifies") {
        unordered_map<string, unordered_set<string>> expectedProcModifies = {
                {"main", {"x", "y", "a", "b"}}
        };
        REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    }

    SECTION("Statement Modifies") {
        unordered_map<string, unordered_set<string>> expectedStmtModifies = {
                {"1", {"x"}}, {"2", {"y"}}, {"3", {"a"}},
                {"4", {"a"}}, {"5", {"b"}}, {"6", {"b"}}
        };
        REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    }
}

TEST_CASE("ModifiesExtractor: Nested") {
    string s = "procedure main {\n"
               "\twhile (b != 1) {\n"
               "\t\tread x;\n"
               "\t\tif (f > 1) then {\n"
               "\t\t\ty = z + 1;\n"
               "\t\t} else {\n"
               "\t\t\ta = b + 2;\n"
               "\t\t}\n"
               "\t}\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

    SECTION("Procedure Modifies") {
        unordered_map<string, unordered_set<string>> expectedProcModifies = {
                {"main", {"x", "y", "a"}}
        };
        REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    }

    SECTION("Statement Modifies") {
        unordered_map<string, unordered_set<string>> expectedStmtModifies = {
                {"1", {"x", "y", "a"}}, {"2", {"x"}}, {"3", {"y", "a"}},
                {"4", {"y"}}, {"5", {"a"}}
        };
        REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    }
}

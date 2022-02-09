#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/FollowsExtractor.h"
#include "SimpleParser/Parser.h"

using namespace std;

TEST_CASE("FollowsExtractor: Non-nested") {
    string s = "procedure main {\n"
               "\tread x;\n"
               "\ty = z + 1;\n"
               "\twhile (b != 1) {\n"
               "\t\ta = 2;\n"
               "\t\tb = 4;\n"
               "\t}\n"
               "\tif (f > 1) then {\n"
               "\t\tb = 3;\n"
               "\t} else {\n"
               "\t\tprint a;\n"
               "\t\tread b;\n"
               "\t}\n"
               "\tprint k;\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3", "6", "10"}}, {"2", {"3", "6", "10"}}, {"3", {"6", "10"}},
            {"6", {"10"}}, {"4", {"5"}}, {"8", {"9"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
}

TEST_CASE("FollowsExtractor: Nested") {
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
    Parser p;
    TNode* ast = p.parseProgram(s);
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"2", {"3"}}, {"4", {"5"}}, {"6", {"7"}}, {"8", {"9", "10"}}, {"9", {"10"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
}


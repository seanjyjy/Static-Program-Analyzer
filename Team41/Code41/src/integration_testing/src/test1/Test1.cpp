#include "catch.hpp"
#include "DesignExtractor/DesignExtractor.h"
#include "SimpleParser/Parser.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "Common/FileReader.h"
#include <filesystem>

using namespace std;

TEST_CASE("Test 1") {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    cwd /= "src/integration_testing/src/test1/Test1.txt";
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    Parser p;
    TNode* ast = p.parseProgram(fileContent);
    PKB pkbManager = PKB();
    DesignExtractor designExtractor(ast, &pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, \"i\")";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(&pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"3", "4", "11"};
        REQUIRE(result == answer);
    }

    SECTION("Query 2") {
        string query = "assign a;\n"
                       "Select a pattern a(\"z\", _\"x\"_)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(&pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"7", "9"};
        REQUIRE(result == answer);
    }

    SECTION("Query 3") {
        string query = "assign a;\n"
                       "Select a pattern a(_, _\"z+x\"_)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(&pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"8", "9"};
        REQUIRE(result == answer);
    }

    SECTION("Query 4") {
        string query = "assign a; variable v;\n"
                       "Select v pattern a(v, _\"z+x\"_)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(&pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"y", "z"};
        REQUIRE(result == answer);
    }
}
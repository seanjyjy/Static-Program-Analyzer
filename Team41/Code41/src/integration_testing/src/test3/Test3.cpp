#include "catch.hpp"
#include "DesignExtractor/DesignExtractor.h"
#include "SimpleParser/Parser.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "Common/FileReader.h"
#include <filesystem>

using namespace std;

TEST_CASE("Test 3") {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    cwd /= "src/integration_testing/src/test3/Test3.txt";
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    Parser p;
    TNode* ast = p.parseProgram(fileContent);
    PKB* pkbManager = new PKB();
    DesignExtractor designExtractor(ast, pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        string query = "variable v;\n"
                       "Select v such that Modifies (6, v)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"count"};
        REQUIRE(result == answer);
    }

    SECTION("Query 2") {
        string query = "variable v;\n"
                       "Select v such that Uses (15, v)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"cenX", "cenY"};
        REQUIRE(result == answer);
    }

    SECTION("Query 3") {
        string query = "variable v; procedure p;\n"
                       "Select p such that  Modifies (p, \"x\")";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"computeCentroid"};
        REQUIRE(result == answer);
    }
}
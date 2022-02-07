#include "catch.hpp"
#include "DesignExtractor/DesignExtractor.h"
#include "SimpleParser/Parser.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "Common/FileReader.h"
#include <filesystem>

using namespace std;

TEST_CASE() {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    cwd /= "src/integration_testing/src/test1/Test1.txt";
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    Parser p = Parser{fileContent};
    TNode* ast = p.parse();
    PKB* pkbManager = new PKB();
    DesignExtractor designExtractor(ast, pkbManager);
    designExtractor.extractDesign();

    SECTION("Query 1") {
        string query = "stmt s;\n"
                       "Select s such that Modifies(s, \"i\")";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(pkbManager);
        printf("test first element lerft variable: %d\n", queryObject->clauses[0].getLeftClauseVariable().type);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"3", "4", "11", "12", "13", "14", "17"};
        REQUIRE(result == answer);
    }

    SECTION("Query 2") {
        string query = "variable v;\n"
                       "Select v such that Uses(\"p\", v)";

        QueryParser qp = QueryParser{query};
        QueryObject* queryObject = qp.parse();
        QueryEvaluator queryEvaluator(pkbManager);
        unordered_set<string> result = queryEvaluator.evaluateQuery(queryObject);

        unordered_set<string> answer{"i", "x", "y", "z"};
        REQUIRE(result == answer);
    }
}
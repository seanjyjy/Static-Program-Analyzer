#include "catch.hpp"
#include "SimpleParser/Parser.h"
#include "Common/TreeUtils.h"
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryParser.h"

TEST_CASE("Integration test for simple source and PQL") {
    string s = "procedure main {"
               "x = y;"
               "g = a;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);

    PKBManager* pkbManager = new PKBManager();
    DesignExtractor designExtractor(ast, pkbManager);
    designExtractor.extractDesign();

    string query = "variable v; assign a;\n"
               "Select v such that Uses(a, v)";

    QueryParser qp = QueryParser{query};
    QueryObject* queryObject = qp.parse();
    QueryEvaluator queryEvaluator(pkbManager);
    QueryResult queryResult = queryEvaluator.evaluateQuery(queryObject);
    unordered_set<string> result = QueryProjector(queryResult, pkbManager).getResult();
    string stringRepresentation;
    for (auto& res : result) {
        stringRepresentation += res;
        stringRepresentation += ", ";
    }
    stringRepresentation = stringRepresentation.substr(0, stringRepresentation.size() - 2);
    printf("Answer: %s", stringRepresentation.c_str());

    queryObject->cleanUp();
    delete queryObject;
    delete pkbManager;
    delete ast;
}
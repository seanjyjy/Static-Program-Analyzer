#include "catch.hpp"
#include "SimpleParser/Parser.h"
#include "Common/TreeUtils.h"
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryParser.h"
#include <iterator>


TEST_CASE("Integration test for simple source and PQL") {
    string s = "procedure main {"
               "x = y;"
               "g = a;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    ast->printRecursive();

    PKBManager* pkbManager = new PKBManager();
    DesignExtractor designExtractor(ast, pkbManager);
    designExtractor.extractDesign();

    string query = "variable v; assign a;\n"
               "Select v such that Uses(a, v)";

    // TODO: change the query test once hardy is done
    QueryParser qp = QueryParser{query};
    QueryObject* queryObject = qp.parse();
    QueryEvaluator queryEvaluator(pkbManager);

    // SIMULATE FEEDING IN QUERY OBJECT FIRST
    vector<QueryClause> queryClauseList;
    string a = "a";
    string v = "v";
    ClauseVariable left(ClauseVariable::variable_type::synonym, a, QueryDeclaration::NONE);
    ClauseVariable right(ClauseVariable::variable_type::synonym, v, QueryDeclaration::NONE);
    QueryClause queryClause(QueryClause::clause_type::usesS, left, right);
    queryClauseList.push_back(queryClause);

    vector<QueryDeclaration> declarationList;
    QueryDeclaration queryDeclaration1(QueryDeclaration::design_entity_type::ASSIGN, a);
    QueryDeclaration queryDeclaration2(QueryDeclaration::design_entity_type::VARIABLE, v);
    declarationList.push_back(queryDeclaration1);
    declarationList.push_back(queryDeclaration2);

    QueryDeclaration selectSynonym(QueryDeclaration::design_entity_type::VARIABLE, v);

    vector<PatternClause> pc;
    auto *queryObjectMock = new QueryObject(declarationList, queryClauseList, pc, selectSynonym, true);
    QueryProjector projector = queryEvaluator.evaluateQuery(queryObjectMock);
    unordered_set<string> result = projector.getResult();
    string stringRepresentation;
    for (auto& res : result) {
        stringRepresentation += res;
        stringRepresentation += ", ";
    }
    stringRepresentation = stringRepresentation.substr(0, stringRepresentation.size() - 2);
    printf("Answer: %s", stringRepresentation.c_str());

    delete queryObject;
    delete queryObjectMock;
    delete pkbManager;
    delete ast;
}
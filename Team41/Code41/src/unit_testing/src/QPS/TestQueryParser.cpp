#include "catch.hpp"

#include "QPS/QueryParser.h"

using namespace std;

TEST_CASE("QPS: Parser_VALID") {
    SECTION("VALID DECLARATION w/o such that") {
        string s = "variable v;\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::VARIABLE;
        bool synMatch = qo->declarations.at(0).synonym == "v";
        bool selectMatch = qo->selectSynonym.synonym == "v";

        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(selectMatch);
    }
    SECTION("UsesS") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool typeMatch = qo->declarations.at(1).type == QueryDeclaration::ASSIGN;
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::usesS;
        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("UsesS with quote variables") {
        string s = "stmt s;\n"
                   "Select s such that Uses(s, \"bloody\")";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::usesS;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
    }
    SECTION("ModifiesP") {
        string s = "procedure p; variable v;\n"
                   "Select v such that Modifies(p, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool typeMatch = qo->declarations.at(1).type == QueryDeclaration::VARIABLE;
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::modifiesP;
        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("ModifiesP with quote proc names") {
        string s = "variable v;\n"
                   "Select v such that Modifies(\"someProcName\", v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::VARIABLE;
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::modifiesP;
        bool selectMatch = qo->selectSynonym.synonym == "v";

        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
        REQUIRE(selectMatch);
    }
    SECTION("Call case") {
        string s = "call c; variable v;\n"
                   "Select v such that Uses(c, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::CALL;
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::usesS;

        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("ClauseVariable") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::usesS;
        bool vMatch = qo->clauses.at(0).getLeftClauseVariable().type == ClauseVariable::synonym;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("ClauseVariable wildcard") {
        string s = "variable v;\n"
                   "Select v such that Modifies(_, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::modifiesS;
        bool vMatch = qo->clauses.at(0).getLeftClauseVariable().type == ClauseVariable::wildcard;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("ClauseVariable integer") {
        string s = "variable v;\n"
                   "Select v such that Modifies(42, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::modifiesS;
        bool vMatch = qo->clauses.at(0).getLeftClauseVariable().type == ClauseVariable::integer;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
}

TEST_CASE("QPS: Parser_INVALID") {
    SECTION("INVALID DECLARATION such that bad clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("INVALID DECLARATION such that no clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("INVALID DECLARATION malformed such that") {
        string s = "variable v; assign a;\n"
                   "Select v suck that Uses(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("INVALID: select instead of Select") {
        string s = "variable v; assign a;\n"
                   "select v such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("INVALID: Empty query") {
        string s = "";
        QueryParser qp = QueryParser{s};
        qp.parse();
        REQUIRE(true);
    }
    SECTION("INVALID: No Select clause") {
        string s = "variable v; assign a;";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("INVALID: Undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select f such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
}

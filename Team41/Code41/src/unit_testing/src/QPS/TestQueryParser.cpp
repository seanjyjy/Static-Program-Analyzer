#include "catch.hpp"

#include "QPS/QueryParser.h"

using namespace std;

TEST_CASE("QPS: Parser_VALID") {
    SECTION("Missing such that") {
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

        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::NONE);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
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
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::CALL);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
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
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
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
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::NONE);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
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
    SECTION("Full Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("Sub Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (\"x\", _\"sub + 1\"_)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isSubPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isIdentifier());
    }
    SECTION("Wildcard Pattern") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (v, _)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isSynonym());
    }
    SECTION("TYPE syn1, syn2;") {
        string s = "variable v, v1, v2; assign a, a1;\n"
                   "Select v";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(qo->isQueryValid);
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::VARIABLE;
        bool synMatch = qo->declarations.at(0).synonym == "v";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(1).type == QueryDeclaration::VARIABLE;
        synMatch = qo->declarations.at(1).synonym == "v1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(2).type == QueryDeclaration::VARIABLE;
        synMatch = qo->declarations.at(2).synonym == "v2";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(3).type == QueryDeclaration::ASSIGN;
        synMatch = qo->declarations.at(3).synonym == "a";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(4).type == QueryDeclaration::ASSIGN;
        synMatch = qo->declarations.at(4).synonym == "a1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
    }
    SECTION("Crazy spaces") {
        string s = "variable           v,v1,      v2  ; assign          a, a1      ;\n"
                   "Select         v pattern       a (   _    ,     \"count + 1\")      ";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(qo->isQueryValid);
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::VARIABLE;
        bool synMatch = qo->declarations.at(0).synonym == "v";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(1).type == QueryDeclaration::VARIABLE;
        synMatch = qo->declarations.at(1).synonym == "v1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(2).type == QueryDeclaration::VARIABLE;
        synMatch = qo->declarations.at(2).synonym == "v2";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(3).type == QueryDeclaration::ASSIGN;
        synMatch = qo->declarations.at(3).synonym == "a";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->declarations.at(4).type == QueryDeclaration::ASSIGN;
        synMatch = qo->declarations.at(4).synonym == "a1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("Whole shebang 1") {
        string s = "assign a; variable v;\n"
                   "Select a such that Uses(a, v) pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("Whole shebang 2") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (_, \"count + 1\") such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
}

TEST_CASE("QPS: Parser_INVALID") {
    SECTION("Bad declaration type") {
        string s = "variable v; asinine a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("synonym beginning with '_'") {
        string s = "variable _vlad; assign a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("synonym beginning with '0-9'") {
        string s = "variable v; assign 69a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("such that bad clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("such that no clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("No ClauseVariable left") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(, v)";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("No ClauseVariable right") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, )";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Malformed such that") {
        string s = "variable v; assign a;\n"
                   "Select v suck that Uses(a, v)";
        QueryParser a = QueryParser{s};

        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("select instead of Select") {
        string s = "variable v; assign a;\n"
                   "select v such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Empty query") {
        string s = "";
        QueryParser qp = QueryParser{s};
        qp.parse();
        REQUIRE(true);
    }
    SECTION("No Select clause") {
        string s = "variable v; assign a;";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Select undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select f such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("clause undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(leftBad, v)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("clause undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, rightBad)";

        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("pattern undeclared LHS") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) pattern a (leftPatBad, _)";
        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("pattern no synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) pattern (leftPatBad, _)";
        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("pattern misspelled") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) paddern a (v, _)";
        QueryParser a = QueryParser{s};
        QueryObject *qo = a.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Missing semi colon 1") {
        string s = "variable v\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Missing semi colon 2") {
        string s = "variable v            Select v";

        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();

        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Rogue semicolons in declarations 1") {
        string s = ";;;variable v; Select v";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Rogue semicolons in declarations 2") {
        string s = "variable ;;; ; ;v; Select v";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Rogue semicolons in select") {
        string s = "variable v; Select ;; ; v";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
    SECTION("Rogue semicolons at query end") {
        string s = "variable v; Select v;";
        QueryParser qp = QueryParser{s};
        QueryObject *qo = qp.parse();
        REQUIRE(!qo->isQueryValid);
    }
}

#include "catch.hpp"

#include "QPS/QueryParser.h"

using namespace std;

TEST_CASE("QPS: Parser_VALID") {
    QueryObject *qo = nullptr;
    SECTION("Missing such that") {
        string s = "variable v;\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
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
        qo = qp.parse();
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
        qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::usesS;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
    }
    SECTION("ModifiesP") {
        string s = "procedure p; variable v;\n"
                   "Select v such that Modifies(p, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
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
        qo = qp.parse();
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
        qo = qp.parse();
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
        qo = qp.parse();
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
        qo = qp.parse();
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
        qo = qp.parse();
        bool clauseMatch = qo->clauses.at(0).type == QueryClause::modifiesS;
        bool vMatch = qo->clauses.at(0).getLeftClauseVariable().type == ClauseVariable::integer;
        REQUIRE(qo->isQueryValid);
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("Assign Full Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("Assign Sub Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (\"x\", _\"sub + 1\"_)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isSubPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isIdentifier());
    }
    SECTION("Assign Wildcard Pattern") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (v, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isSynonym());
        REQUIRE(qo->patternClauses.at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("If Pattern identifier condVar") {
        string s = "if ifs;\n"
                   "Select ifs pattern ifs (\"v\", _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::IF);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 2);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isIdentifier());
    }
    SECTION("If Pattern synonym condVar") {
        string s = "if ifs; variable v;\n"
                   "Select ifs pattern ifs (v, _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::IF);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 2);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isSynonym());
        REQUIRE(qo->patternClauses.at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("If Pattern Wildcard condVar") {
        string s = "if ifs;\n"
                   "Select ifs pattern ifs (_, _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::IF);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 2);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("While Pattern identifier condVar") {
        string s = "while w;\n"
                   "Select w pattern w (\"v\", _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::WHILE);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isIdentifier());
    }
    SECTION("While Pattern synonym condVar") {
        string s = "while w; variable v;\n"
                   "Select w pattern w (v, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::WHILE);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isSynonym());
        REQUIRE(qo->patternClauses.at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("While Pattern Wildcard condVar") {
        string s = "while w;\n"
                   "Select w pattern w (_, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::WHILE);
        REQUIRE(qo->patternClauses.at(0).getRHS().size() == 1);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("TYPE syn1, syn2;") {
        string s = "variable v, v1, v2; assign a, a1;\n"
                   "Select v";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

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
        qo = qp.parse();

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
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
    }
    SECTION("Whole shebang 1") {
        string s = "assign a; variable v;\n"
                   "Select a such that Uses(a, v) pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("Whole shebang 2") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (_, \"count + 1\") such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->patternClauses.at(0).getSynonym().type == QueryDeclaration::ASSIGN);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->patternClauses.at(0).getLHS().isWildCard());
        REQUIRE(qo->clauses.at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->clauses.at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("SelectTarget test") {
        string s = "variable v;\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->declarations.at(0).type == QueryDeclaration::VARIABLE;
        bool synMatch = qo->declarations.at(0).synonym == "v";
        bool selectMatch = qo->selectTarget.tuple.at(0).getSynonym().synonym == "v";
        REQUIRE(qo->isQueryValid);
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(selectMatch);
    }
    SECTION("Select BOOLEAN") {
        string s = "Select BOOLEAN such that Next* (2, 9)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->selectTarget.isBoolean());
        REQUIRE(qo->clauses.at(0).type == QueryClause::nextT);
    }
    SECTION("Select attribute") {
        string s = "procedure p, q;\n"
                   "Select p.procName such that Calls (p, q)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->selectTarget.isTuple());
        REQUIRE(qo->selectTarget.tuple.at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->selectTarget.tuple.at(0).getSynonym().synonym == "p");
        REQUIRE(qo->selectTarget.tuple.at(0).getAttr() == Selectable::PROC_NAME);
        REQUIRE(qo->clauses.at(0).type == QueryClause::calls);
    }
    SECTION("Select single elem tuple") {
        string s = "procedure p, q;\n"
                   "Select <p.procName> such that Calls (p, q)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->selectTarget.isTuple());
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(0).getSynonym().synonym == "p");
        REQUIRE(qo->getSelectables().at(0).getAttr() == Selectable::PROC_NAME);
        REQUIRE(qo->clauses.at(0).type == QueryClause::calls);
    }
    SECTION("Select tuple") {
        string s = "assign a1, a2;\n"
                   "Select <a1, a1.stmt#, a2, a2.stmt#> such that Affects (a1, a2)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->selectTarget.isTuple());
        REQUIRE(qo->getSelectables().at(0).getSynonym().synonym == "a1");
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::SYNONYM);
        REQUIRE(qo->getSelectables().at(1).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(1).getSynonym().synonym == "a1");
        REQUIRE(qo->getSelectables().at(1).getAttr() == Selectable::STMT_NUM);
        REQUIRE(qo->getSelectables().at(2).getSynonym().synonym == "a2");
        REQUIRE(qo->getSelectables().at(2).getType() == Selectable::SYNONYM);
        REQUIRE(qo->getSelectables().at(3).getSynonym().synonym == "a2");
        REQUIRE(qo->getSelectables().at(3).getAttr() == Selectable::STMT_NUM);
    }
    SECTION("such that and") {
        string s = "Select BOOLEAN such that Next* (2, 8) and Next* (8, 9)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->isSelectingBoolean());
        REQUIRE(qo->clauses.at(0).type == QueryClause::nextT);
        REQUIRE(qo->clauses.at(1).type == QueryClause::nextT);
    }
    SECTION("pattern and") {
        string s = "assign a1, a2; while w1, w2;\n"
                   "Select a2 pattern a1 (\"x\", _) and a2 (\"x\", _\"x\"_)\n"
                   "          such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isQueryValid);
        REQUIRE(qo->patternClauses.at(0).getSynonym().synonym == "a1");
        REQUIRE(qo->patternClauses.at(0).getLHS().isIdentifier());
        REQUIRE(qo->patternClauses.at(0).getLHS().getLabel() == "x");
        REQUIRE(qo->patternClauses.at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->patternClauses.at(1).getSynonym().synonym == "a2");
        REQUIRE(qo->clauses.at(0).type == QueryClause::affects);
        REQUIRE(qo->clauses.at(1).type == QueryClause::parentT);
        REQUIRE(qo->clauses.at(2).type == QueryClause::parentT);
        REQUIRE(qo->clauses.at(2).getLeftClauseVariable().getLabel() == "w1");
        REQUIRE(qo->clauses.at(2).getRightClauseVariable().getLabel() == "w2");
    }
    SECTION("stmt boolean select boolean") {
        string s = "stmt BOOLEAN;\n"
                   "Select BOOLEAN";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isSelectingBoolean());
        REQUIRE(qo->selectTarget.tuple.at(0).getSynonym().synonym == "BOOLEAN");
    }
    delete qo;
}

TEST_CASE("QPS: Parser_INVALID") {
    QueryObject *qo = nullptr;
    SECTION("Bad declaration type") {
        string s = "variable v; asinine a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("synonym beginning with '_'") {
        string s = "variable _vlad; assign a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("synonym beginning with '0-9'") {
        string s = "variable v; assign 69a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("such that bad clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("such that no clause") {
        string s = "variable v; assign a;\n"
                   "Select v such that";

        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("No ClauseVariable left") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("No ClauseVariable right") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, )";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Malformed such that") {
        string s = "variable v; assign a;\n"
                   "Select v suck that Uses(a, v)";
        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("select instead of Select") {
        string s = "variable v; assign a;\n"
                   "select v such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Empty query") {
        string s = "";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("No Select clause") {
        string s = "variable v; assign a;";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Select undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select f such that Uses(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("clause undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(leftBad, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("clause undeclared synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, rightBad)";

        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("pattern undeclared LHS") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) pattern a (leftPatBad, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("pattern no synonym") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) pattern (leftPatBad, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("pattern misspelled") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) paddern a (v, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("assign pattern extra arg") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v) pattern a (v, _, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("while pattern extra arg") {
        string s = "variable v; while w;\n"
                   "Select v such that Uses(w, v) pattern w (v, _, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("unknown pattern") {
        string s = "variable v; call c;\n"
                   "Select v such that Uses(c, v) pattern w (c, _)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("while pattern using fullpattern") {
        string s = "variable v; while w;\n"
                   "Select v such that Uses(w, v) pattern w (v, \"x+1\")";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("while pattern using subpattern") {
        string s = "variable v; while w;\n"
                   "Select v such that Uses(w, v) pattern w (v, _\"x+1\"_)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("if pattern using fullpattern") {
        string s = "variable v; if ifs;\n"
                   "Select v such that Uses(ifs, v) pattern ifs (v, \"x+1\")";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("if pattern using subpattern") {
        string s = "variable v; if ifs;\n"
                   "Select v such that Uses(ifs, v) pattern ifs (v, _\"x+1\"_)";
        QueryParser a = QueryParser{s};
        qo = a.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Missing semi colon 1") {
        string s = "variable v\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Missing semi colon 2") {
        string s = "variable v            Select v";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Rogue semicolons in declarations 1") {
        string s = ";;;variable v; Select v";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Rogue semicolons in declarations 2") {
        string s = "variable ;;; ; ;v; Select v";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Rogue semicolons in select") {
        string s = "variable v; Select ;; ; v";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("Rogue semicolons at query end") {
        string s = "variable v; Select v;";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    SECTION("BOOLEAN within tuple") {
        string s = "stmt s;\n"
                   "Select <BOOLEAN, s>";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    delete qo;
}

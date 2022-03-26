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
        bool typeMatch = qo->getDeclarations().at(0).type == QueryDeclaration::VARIABLE;
        bool synMatch = qo->getDeclarations().at(0).synonym == "v";
        bool selectMatch = qo->getSelectables().at(0).getSynonym().synonym == "v";
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(selectMatch);
    }
    SECTION("UsesS") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->getDeclarations().at(1).getType() == QueryDeclaration::ASSIGN;
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::usesS;
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("UsesS with quote variables") {
        string s = "stmt s;\n"
                   "Select s such that Uses(s, \"bloody\")";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::usesS;
        REQUIRE(qo->isValid());
        REQUIRE(clauseMatch);
    }
    SECTION("ModifiesP") {
        string s = "procedure p; variable v;\n"
                   "Select v such that Modifies(p, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->getDeclarations().at(1).getType() == QueryDeclaration::VARIABLE;
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::modifiesP;
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("ModifiesP with quote proc names") {
        string s = "variable v;\n"
                   "Select v such that Modifies(\"someProcName\", v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->getDeclarations().at(0).getType() == QueryDeclaration::VARIABLE;
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::modifiesP;
        bool selectMatch = qo->getSelectables().at(0).getSynonym().synonym == "v";

        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::NONE);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
        REQUIRE(selectMatch);
    }
    SECTION("Call case") {
        string s = "call c; variable v;\n"
                   "Select v such that Uses(c, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->getDeclarations().at(0).getType() == QueryDeclaration::CALL;
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::usesS;
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::CALL);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(clauseMatch);
    }
    SECTION("ClauseVariable") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::usesS;
        bool vMatch = qo->getClauses().at(0).getLeftClauseVariable().getType() == ClauseVariable::synonym;
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->isValid());
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("ClauseVariable wildcard") {
        string s = "variable v;\n"
                   "Select v such that Modifies(_, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::modifiesS;
        bool vMatch = qo->getClauses().at(0).getLeftClauseVariable().getType() == ClauseVariable::wildcard;
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::NONE);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->isValid());
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("ClauseVariable integer") {
        string s = "variable v;\n"
                   "Select v such that Modifies(42, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool clauseMatch = qo->getClauses().at(0).getType() == QueryClause::modifiesS;
        bool vMatch = qo->getClauses().at(0).getLeftClauseVariable().getType() == ClauseVariable::integer;
        REQUIRE(qo->isValid());
        REQUIRE(clauseMatch);
        REQUIRE(vMatch);
    }
    SECTION("Assign Full Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
    }
    SECTION("Assign Sub Pattern") {
        string s = "assign a;\n"
                   "Select a pattern a (\"x\", _\"sub + 1\"_)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isSubPattern());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isIdentifier());
    }
    SECTION("Assign Wildcard Pattern") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (v, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isSynonym());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("If Pattern identifier condVar") {
        string s = "if ifs;\n"
                   "Select ifs pattern ifs (\"v\", _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::IF);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 2);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isIdentifier());
    }
    SECTION("If Pattern synonym condVar") {
        string s = "if ifs; variable v;\n"
                   "Select ifs pattern ifs (v, _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::IF);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 2);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isSynonym());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("If Pattern Wildcard condVar") {
        string s = "if ifs;\n"
                   "Select ifs pattern ifs (_, _, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::IF);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 2);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(1).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
    }
    SECTION("While Pattern identifier condVar") {
        string s = "while w;\n"
                   "Select w pattern w (\"v\", _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::WHILE);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isIdentifier());
    }
    SECTION("While Pattern synonym condVar") {
        string s = "while w; variable v;\n"
                   "Select w pattern w (v, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::WHILE);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isSynonym());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("While Pattern Wildcard condVar") {
        string s = "while w;\n"
                   "Select w pattern w (_, _)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::WHILE);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().size() == 1);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() == nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isWildcard());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
    }
    SECTION("TYPE syn1, syn2;") {
        string s = "variable v, v1, v2; assign a, a1;\n"
                   "Select v";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        bool typeMatch = qo->getDeclarations().at(0).getType() == QueryDeclaration::VARIABLE;
        bool synMatch = qo->getDeclarations().at(0).getSynonym() == "v";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(1).getType() == QueryDeclaration::VARIABLE;
        synMatch = qo->getDeclarations().at(1).getSynonym() == "v1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(2).getType() == QueryDeclaration::VARIABLE;
        synMatch = qo->getDeclarations().at(2).getSynonym() == "v2";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(3).getType() == QueryDeclaration::ASSIGN;
        synMatch = qo->getDeclarations().at(3).getSynonym() == "a";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(4).getType() == QueryDeclaration::ASSIGN;
        synMatch = qo->getDeclarations().at(4).getSynonym() == "a1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
    }
    SECTION("Crazy spaces") {
        string s = "variable           v,v1,      v2  ; assign          a, a1      ;\n"
                   "Select         v pattern       a (   _    ,     \"count + 1\")      ";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        bool typeMatch = qo->getDeclarations().at(0).getType() == QueryDeclaration::VARIABLE;
        bool synMatch = qo->getDeclarations().at(0).getSynonym() == "v";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(1).getType() == QueryDeclaration::VARIABLE;
        synMatch = qo->getDeclarations().at(1).getSynonym() == "v1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(2).getType() == QueryDeclaration::VARIABLE;
        synMatch = qo->getDeclarations().at(2).getSynonym() == "v2";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(3).getType() == QueryDeclaration::ASSIGN;
        synMatch = qo->getDeclarations().at(3).getSynonym() == "a";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        typeMatch = qo->getDeclarations().at(4).getType() == QueryDeclaration::ASSIGN;
        synMatch = qo->getDeclarations().at(4).getSynonym() == "a1";
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
    }
    SECTION("Whole shebang 1") {
        string s = "assign a; variable v;\n"
                   "Select a such that Uses(a, v) pattern a (_, \"count + 1\")";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("Whole shebang 2") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (_, \"count + 1\") such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        REQUIRE(qo->getPatternClauses().at(0).getSynonym().getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).getMiniAST() != nullptr);
        REQUIRE(qo->getPatternClauses().at(0).getRHS().at(0).isFullPattern());
        REQUIRE(qo->getPatternClauses().at(0).getLHS().isWildCard());
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getDesignEntityType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getClauses().at(0).getRightClauseVariable().getDesignEntityType() == QueryDeclaration::VARIABLE);
    }
    SECTION("SelectTarget test") {
        string s = "variable v;\n"
                   "Select v";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        bool typeMatch = qo->getDeclarations().at(0).getType() == QueryDeclaration::VARIABLE;
        bool synMatch = qo->getDeclarations().at(0).getSynonym() == "v";
        bool selectMatch = qo->getSelectables().at(0).getSynonym().getSynonym() == "v";
        REQUIRE(qo->isValid());
        REQUIRE(typeMatch);
        REQUIRE(synMatch);
        REQUIRE(selectMatch);
    }
    SECTION("Select BOOLEAN") {
        string s = "Select BOOLEAN such that Next* (2, 9)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->getSelectTarget().isBoolean());
        REQUIRE(qo->getClauses().at(0).getType() == QueryClause::nextT);
        REQUIRE(qo->isValid());
        delete qo;

        string s2 = "variable v;\n"
                    "Select BOOLEAN such that Uses(\"printResults\", v)";
        QueryParser qp2 = QueryParser{s2};
        qo = qp2.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSelectTarget().isBoolean());
    }
    SECTION("Select attribute") {
        string s = "procedure p, q;\n"
                   "Select p.procName such that Calls (p, q)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->getSelectTarget().isTuple());
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "p");
        REQUIRE(qo->getSelectables().at(0).getAttr() == Selectable::PROC_NAME);
        REQUIRE(qo->getClauses().at(0).getType() == QueryClause::calls);
    }
    SECTION("Select single elem tuple") {
        string s = "procedure p, q;\n"
                   "Select <p.procName> such that Calls (p, q)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSelectTarget().isTuple());
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "p");
        REQUIRE(qo->getSelectables().at(0).getAttr() == Selectable::PROC_NAME);
        REQUIRE(qo->getClauses().at(0).getType() == QueryClause::calls);
    }
    SECTION("Select tuple") {
        string s = "assign a1, a2;\n"
                   "Select <a1, a1.stmt#, a2, a2.stmt#> such that Affects (a1, a2)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSelectTarget().isTuple());
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "a1");
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::SYNONYM);
        REQUIRE(qo->getSelectables().at(1).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(1).getSynonym().getSynonym() == "a1");
        REQUIRE(qo->getSelectables().at(1).getAttr() == Selectable::STMT_NUM);
        REQUIRE(qo->getSelectables().at(2).getSynonym().getSynonym() == "a2");
        REQUIRE(qo->getSelectables().at(2).getType() == Selectable::SYNONYM);
        REQUIRE(qo->getSelectables().at(3).getSynonym().getSynonym() == "a2");
        REQUIRE(qo->getSelectables().at(3).getAttr() == Selectable::STMT_NUM);
    }
    SECTION("such that and") {
        string s = "Select BOOLEAN such that Next* (2, 8) and Next* (8, 9)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        REQUIRE(qo->isSelectingBoolean());
        REQUIRE(qo->getClauses().at(0).getType() == QueryClause::nextT);
        REQUIRE(qo->getClauses().at(1).getType() == QueryClause::nextT);
        ////
        REQUIRE(qo->getSuperClauses().at(0)->isSuchThatClause());
        REQUIRE(qo->getSuperClauses().at(0)->isNextT());
        REQUIRE_FALSE(qo->getSuperClauses().at(0)->hasSynonyms());
        REQUIRE(qo->getSuperClauses().at(1)->isSuchThatClause());
        REQUIRE(qo->getSuperClauses().at(1)->isNextT());
        REQUIRE_FALSE(qo->getSuperClauses().at(1)->hasSynonyms());
    }
    SECTION("pattern and") {
        string s = "assign a1, a2; while w1, w2;\n"
                   "Select a2 pattern a1 (\"x\", _) and a2 (\"x\", _\"x\"_)\n"
                   "          such that Affects (a1, a2) and Parent* (w2, a2) and Parent* (w1, w2)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
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
        REQUIRE(qo->isValid());
        REQUIRE_FALSE(qo->isSelectingBoolean());
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "BOOLEAN");
    }
    SECTION("suchthat-with-suchthat ") {
        string s = "procedure p, q;\n"
                   "Select p such that Calls (p, q) with q.procName=\"Third\" such that Modifies (p, \"i\")";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        ////
        REQUIRE(qo->getClauses().at(0).getType() == QueryClause::calls);
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().isSynonym());
        REQUIRE(qo->getClauses().at(0).getLeftClauseVariable().getLabel() == "p");
        REQUIRE(qo->getClauses().at(1).getType() == QueryClause::modifiesP);
        REQUIRE(qo->getClauses().at(1).getLeftClauseVariable().isSynonym());
        REQUIRE(qo->getClauses().at(1).getLeftClauseVariable().getLabel() == "p");
        REQUIRE(qo->getClauses().at(1).getRightClauseVariable().isIdentifier());
        REQUIRE(qo->getClauses().at(1).getRightClauseVariable().getLabel() == "i");
        ////
        REQUIRE(qo->getWithClauses().at(0).getLeft().getType() == WithVariable::ATTR_REF);
        REQUIRE(qo->getWithClauses().at(0).getLeft().getSynonym().getSynonym() == "q");
        REQUIRE(qo->getWithClauses().at(0).getLeft().getAttr() == WithVariable::PROC_NAME);
        REQUIRE(qo->getWithClauses().at(0).getRight().getType() == WithVariable::IDENT);
        REQUIRE(qo->getWithClauses().at(0).getRight().getIdent() == "Third");
    }
    SECTION("with-integer") {
        string s = "stmt s, s1;\n"
                   "Select s.stmt# such that Follows* (s, s1) with s1.stmt#=10";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "s");
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(0).getAttr() == Selectable::STMT_NUM);
        ////
        REQUIRE(qo->getWithClauses().at(0).getLeft().getType() == WithVariable::ATTR_REF);
        REQUIRE(qo->getWithClauses().at(0).getLeft().getSynonym().getSynonym() == "s1");
        REQUIRE(qo->getWithClauses().at(0).getLeft().getAttr() == WithVariable::STMT_NUM);
        REQUIRE(qo->getWithClauses().at(0).getRight().getType() == WithVariable::INTEGER);
        REQUIRE(qo->getWithClauses().at(0).getRight().getInteger() == 10);
        REQUIRE(qo->getWithClauses().at(0).getRight().getIntegerAsString() == "10");
    }
    SECTION("superClause test") {
        string s = "stmt s, s1; while w; variable v;\n"
                   "Select s.stmt# such that Follows* (s, s1) with s1.stmt#=10 pattern w (v, _)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSelectables().at(0).getSynonym().getSynonym() == "s");
        REQUIRE(qo->getSelectables().at(0).getType() == Selectable::ATTR_REF);
        REQUIRE(qo->getSelectables().at(0).getAttr() == Selectable::STMT_NUM);
        ////
        REQUIRE(qo->getWithClauses().at(0).getLeft().getType() == WithVariable::ATTR_REF);
        REQUIRE(qo->getWithClauses().at(0).getLeft().getSynonym().getSynonym() == "s1");
        REQUIRE(qo->getWithClauses().at(0).getLeft().getAttr() == WithVariable::STMT_NUM);
        REQUIRE(qo->getWithClauses().at(0).getRight().getType() == WithVariable::INTEGER);
        REQUIRE(qo->getWithClauses().at(0).getRight().getInteger() == 10);
        REQUIRE(qo->getWithClauses().at(0).getRight().getIntegerAsString() == "10");
        ////
        // QueryClause super clause
        REQUIRE(qo->getSuperClauses().at(0)->isSuchThatClause());
        REQUIRE(qo->getSuperClauses().at(0)->isFollowsT());
        REQUIRE(qo->getSuperClauses().at(0)->hasSynonyms());
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().size() == 2);
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(0).getType() == QueryDeclaration::STMT);
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(0).getSynonym() == "s");
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(1).getType() == QueryDeclaration::STMT);
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(1).getSynonym() == "s1");
        // WithClause super clause
        REQUIRE(qo->getSuperClauses().at(1)->isWithClause());
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().size() == 1); // Only LHS gets registered
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(0).getType() == QueryDeclaration::STMT);
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(0).getSynonym() == "s1");
        // PatternClause super clause
        REQUIRE(qo->getSuperClauses().at(2)->isPatternClause());
        REQUIRE(qo->getSuperClauses().at(2)->getSynonyms().size() == 2);
        REQUIRE(qo->getSuperClauses().at(2)->getSynonyms().at(0).getType() == QueryDeclaration::WHILE);
        REQUIRE(qo->getSuperClauses().at(2)->getSynonyms().at(0).getSynonym() == "w");
        REQUIRE(qo->getSuperClauses().at(2)->getSynonyms().at(1).getType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->getSuperClauses().at(2)->getSynonyms().at(1).getSynonym() == "v");
    }
    SECTION("Whole shebang 3") {
        string s = "assign a; variable v;\n"
                   "Select a pattern a (_, \"count + 1\") such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qo = qp.parse();

        REQUIRE(qo->isValid());
        // PatternClause super clause
        REQUIRE(qo->getSuperClauses().at(0)->isPatternClause());
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().size() == 1);
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(0).getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getSuperClauses().at(0)->getSynonyms().at(0).getSynonym() == "a");
        // QueryClause super clause
        REQUIRE(qo->getSuperClauses().at(1)->isSuchThatClause());
        REQUIRE(qo->getSuperClauses().at(1)->isUsesS());
        REQUIRE(qo->getSuperClauses().at(1)->hasSynonyms());
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().size() == 2);
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(0).getType() == QueryDeclaration::ASSIGN);
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(0).getSynonym() == "a");
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(1).getType() == QueryDeclaration::VARIABLE);
        REQUIRE(qo->getSuperClauses().at(1)->getSynonyms().at(1).getSynonym() == "v");
    }
    SECTION("SuperClause wrapper hash test") {
        string s = "stmt s, s1, s2; while w; variable v;\n"
                   "Select s.stmt# such that Follows* (s, s1) with s1.stmt#=10 pattern w (v, _) such that Follows*(s, s1) and Follows*(s1, s2)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSuperClauses().at(0)->isSuchThatClause());
        REQUIRE(qo->getSuperClauses().at(0)->isFollowsT());
        int h0 = qo->getSuperClauses().at(0)->hash();
        int h1 = qo->getSuperClauses().at(1)->hash();
        int h2 = qo->getSuperClauses().at(2)->hash();
        REQUIRE(h0 != h1);
        REQUIRE(h0 != h2);
        REQUIRE(h1 != h2);
        REQUIRE(qo->getSuperClauses().at(0)->getSuchThatClause().getType() == QueryClause::followsT);
    }
    SECTION("SuperClause wrapper equivalence test") {
        string s = "stmt s, s1, s2; while w; variable v; assign a;\n"
                   "Select s.stmt# such that Follows* (s, s1) and Follows*(s, s1) and Follows*(s1, s2) and Uses(a, v)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(1)));
        REQUIRE_FALSE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(2)));
        REQUIRE_FALSE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(3)));

        s = "stmt s, s1, s2; while w; variable v; assign a;\n"
            "Select s.stmt# with s1.stmt#=10 with s1.stmt#=420 with s1.stmt#=10";
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(2)));
        REQUIRE_FALSE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(1)));

        s = "stmt s; if ifs; while w; variable v; assign a;\n"
            "Select s.stmt# pattern w (v, _) and w (v, _) and ifs (v, _, _)";
        qo = qp.parse();
        REQUIRE(qo->isValid());
        REQUIRE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(1)));
        REQUIRE_FALSE(qo->getSuperClauses().at(0)->equals(*qo->getSuperClauses().at(2)));

        SuperClause *sc1 = qo->getSuperClauses().at(0);
        SuperClause *sc2 = qo->getSuperClauses().at(1);
        REQUIRE((*sc1) == (*sc2));
    }
    delete qo;
}

TEST_CASE("QPS: Parser_INVALID") {
    QueryObject *qo = nullptr;
//    SECTION("Repeated declaration name") {
//        string s = "variable v; assign v;\n"
//                   "Select v";
//
//        QueryParser a = QueryParser{s};
//        qo = a.parse();
//
//        REQUIRE_FALSE(qo->isValid());
//    }
    SECTION("Bad declaration type") {
        string s = "variable v; asinine a;\n"
                   "Select v such that Kills(a, v)";

        QueryParser a = QueryParser{s};
        qo = a.parse();

        REQUIRE_FALSE(qo->isValid());
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
    SECTION("Rogue semicolons at query end 2") {
        string s = "stmt s; Select s;";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "stmt s, s1; Select <s1, s>;";
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "stmt s;\n"
            "Select BOOLEAN;";
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "stmt s, s1, s2; while w; variable v; Select s.stmt#;";
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "assign a; variable v;\n"
            "Select a pattern a (_, \"count + 1\") such that Uses(a, v);";
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "assign a; variable v;\n"
            "Select a pattern a (_, \"count + 1\");";
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
    SECTION("Integers with leading 0") {
        string s = "stmt s;\n"
                   "Select BOOLEAN such that Follows(01, _)";
        QueryParser qp = QueryParser{s};
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);

        s = "stmt s, s1, s2; while w; variable v;\n"
                   "Select s.stmt# such that Follows* (s, s1) with s1.stmt#=069 pattern w (v, _) such that Follows*(s, s1) and Follows*(s1, s2)";
        qo = qp.parse();
        REQUIRE_FALSE(qo->isQueryValid);
    }
    delete qo;
}

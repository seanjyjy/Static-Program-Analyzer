#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/AssignPatternTable.h"
#include "QPS/Evaluator/AssignPatternEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "./TableTestUtils.h"
#include "../Common/AstBuilder/TestAstBuilderUtils.h"

TEST_CASE("Evaluator: Assign pattern evaluator") {
    PKBManager *pkbManager = new PKBManager();
    // z = x + 1;
    // x = a + b;
    string lines[] = {"1", "2", "3", "4"};
    string ASSIGN_SYN_LBL = "a1";
    string VAR_SYN_LBL = "v1";
    string vars[] = {"var1", "var2"};
    string xml[] = {"<plus><plus><var name=x></var><var name=y></var></plus><var name=z></var></plus>",
                    "<plus><var name=x></var><var name=y></var></plus>",
                    "<const val=2></const>",
                    "<minus><plus><var name=x></var><times><var name=x></var><var name=y></var></times></plus><const val=20></const></minus>"};
    string expr[] = {"x+y+z", "x+y", "2", "x+y*z-20"};

    QueryDeclaration assignSyn(QueryDeclaration::ASSIGN, ASSIGN_SYN_LBL);
    ClauseVariable variableSyn(ClauseVariable::synonym, VAR_SYN_LBL, QueryDeclaration::VARIABLE);

    ClauseVariable identifierV1(ClauseVariable::identifier, vars[0], QueryDeclaration::VARIABLE);
    ClauseVariable identifierV2(ClauseVariable::identifier, vars[1], QueryDeclaration::VARIABLE);

    ClauseVariable wildcard(ClauseVariable::wildcard, "_", QueryDeclaration::VARIABLE);

    TNode *node1 = TestAstBuilderUtils::parseXml(xml[0]);
    TNode *node2 = TestAstBuilderUtils::parseXml(xml[1]);
    TNode *node3 = TestAstBuilderUtils::parseXml(xml[2]);
    TNode *node4 = TestAstBuilderUtils::parseXml(xml[3]);

    PatternVariable patternFP1(PatternVariable::fullpattern, node1); // x+y+z
    PatternVariable patternFP2(PatternVariable::fullpattern, node2); // x+y
    PatternVariable patternFP3(PatternVariable::fullpattern, node4); // x+y*z-20

    PatternVariable subpatternSP1(PatternVariable::subpattern, node2); // _x+y_
    PatternVariable subpatternSP2(PatternVariable::subpattern, node3); // _2_

    PatternVariable patternWildCard(PatternVariable::wildcard, nullptr);

    // var1 = x+y+z;
    pkbManager->registerAssignPattern(lines[0], vars[0], node1);
    // var2 = x+y;
    pkbManager->registerAssignPattern(lines[1], vars[1], node2);
    // var1 = 2;
    pkbManager->registerAssignPattern(lines[2], vars[0], node3);
    // var2 = x+y*z-20;
    pkbManager->registerAssignPattern(lines[3], vars[1], node4);
    for (string l: lines) {
        pkbManager->registerAssignStmt(l);
    }

    pkbManager->registerVariable(vars[0]);
    pkbManager->registerVariable(vars[1]);
    pkbManager->registerModifiesS(lines[0], vars[0]);
    pkbManager->registerModifiesS(lines[1], vars[1]);
    pkbManager->registerModifiesS(lines[2], vars[0]);
    pkbManager->registerModifiesS(lines[3], vars[1]);

    SECTION("Synonym FullPattern Pair") {
        // "x+y+z" ?
        PatternClause patternClause1(assignSyn, variableSyn, vector<PatternVariable>({patternFP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        // "x+y*z-20" ?
        PatternClause patternClause2(assignSyn, variableSyn, vector<PatternVariable>({patternFP3}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[3], vars[1]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("Synonym SubPattern Pair") {
        // _x+y_ ?
        PatternClause patternClause1(assignSyn, variableSyn, vector<PatternVariable>({subpatternSP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]}, {lines[1], vars[1]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        // _2_ ?
        PatternClause patternClause2(assignSyn, variableSyn, vector<PatternVariable>({subpatternSP2}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[2], vars[0]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("Synonym WildCard Pair") {
        PatternClause patternClause1(assignSyn, variableSyn, vector<PatternVariable>({patternWildCard}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]}, {lines[1], vars[1]}, {lines[2], vars[0]}, {lines[3], vars[1]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Identifier FullPattern Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, vector<PatternVariable>({patternFP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(assignSyn, identifierV1, vector<PatternVariable>({patternFP2}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows2{};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("Identifier SubPattern Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, vector<PatternVariable>({subpatternSP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(assignSyn, identifierV1, vector<PatternVariable>({subpatternSP2}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[2]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("Identifier WildCard Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, vector<PatternVariable>({patternWildCard}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}, {lines[2]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(assignSyn, identifierV2, vector<PatternVariable>({patternWildCard}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[1]}, {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("WildCard FullPattern Pair") {
        PatternClause patternClause1(assignSyn, wildcard, vector<PatternVariable>({patternFP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(assignSyn, wildcard, vector<PatternVariable>({patternFP3}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("WildCard SubPattern Pair") {
        PatternClause patternClause1(assignSyn, wildcard, vector<PatternVariable>({subpatternSP1}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}, {lines[1]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(assignSyn, wildcard, vector<PatternVariable>({subpatternSP2}));
        Table *table2 = AssignPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[2]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("WildCard WildCard Pair") {
        PatternClause patternClause1(assignSyn, wildcard, vector<PatternVariable>({patternWildCard}));
        Table *table1 = AssignPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{ASSIGN_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]}, {lines[1]}, {lines[2]}, {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Semantically & Syntactically Invalid") {
        ClauseVariable procSyn(ClauseVariable::synonym, "proc", QueryDeclaration::PROCEDURE);
        PatternClause patternClause1(assignSyn, procSyn, vector<PatternVariable>({patternFP1}));
        REQUIRE_THROWS(AssignPatternEvaluator(pkbManager).evaluate(patternClause1));

        ClauseVariable readSyn(ClauseVariable::synonym, "proc", QueryDeclaration::READ);
        PatternClause patternClause2(assignSyn, readSyn, vector<PatternVariable>({patternFP1}));
        REQUIRE_THROWS(AssignPatternEvaluator(pkbManager).evaluate(patternClause2));
    }

    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete pkbManager;
}
#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/WhilePatternTable.h"
#include "QPS/Evaluator/WhilePatternEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "./TableTestUtils.h"
#include "../Common/AstBuilder/TestAstBuilderUtils.h"

TEST_CASE("Evaluator: While pattern evaluator") {
    PKBManager *pkbManager = new PKBManager();
    string lines[] = {"1", "2", "3", "4"};
    string WHILE_SYN_LBL = "whiles";
    string VAR_SYN_LBL = "v1";
    string vars[] = {"var0", "var1", "var2"};

    QueryDeclaration whileSyn(QueryDeclaration::WHILE, WHILE_SYN_LBL);

    ClauseVariable identifierV0(ClauseVariable::identifier, vars[0], QueryDeclaration::VARIABLE);
    ClauseVariable identifierV1(ClauseVariable::identifier, vars[1], QueryDeclaration::VARIABLE);
    ClauseVariable variableSyn(ClauseVariable::synonym, VAR_SYN_LBL, QueryDeclaration::VARIABLE);
    ClauseVariable wildcard(ClauseVariable::wildcard, "_", QueryDeclaration::VARIABLE);

    PatternVariable patternWildCard(PatternVariable::wildcard, nullptr);
    PatternVariable patternFP(PatternVariable::fullpattern, nullptr);
    PatternVariable patternSP(PatternVariable::subpattern, nullptr);

    pkbManager->registerVariable(vars[0]);
    pkbManager->registerVariable(vars[1]);
    pkbManager->registerVariable(vars[2]);
    pkbManager->registerWhilePattern(lines[0], vars[0]); // 0: v0
    pkbManager->registerWhilePattern(lines[1], vars[1]); // 1: v1
    pkbManager->registerWhilePattern(lines[2], vars[2]); // 2: v2
    pkbManager->registerWhilePattern(lines[3], vars[0]); // 3: v0
    pkbManager->registerWhilePattern(lines[3], vars[1]); // 3: v1
    pkbManager->registerWhileStmt(lines[0]);
    pkbManager->registerWhileStmt(lines[1]);
    pkbManager->registerWhileStmt(lines[2]);
    pkbManager->registerWhileStmt(lines[3]);

    SECTION("Synonym WildCards Pair") {
        PatternClause patternClause1(whileSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = WhilePatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{WHILE_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]},
                                             {lines[1], vars[1]},
                                             {lines[2], vars[2]},
                                             {lines[3], vars[0]},
                                             {lines[3], vars[1]}};

        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Identifier WildCards Pair") {
        PatternClause patternClause1(whileSyn, identifierV0, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = WhilePatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{WHILE_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(whileSyn, identifierV1, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table2 = WhilePatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{WHILE_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[1]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("WildCard WildCards Pair") {
        PatternClause patternClause1(whileSyn, wildcard, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = WhilePatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{WHILE_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[1]},
                                             {lines[2]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Semantically & Syntactically Invalid") {
        ClauseVariable procSyn(ClauseVariable::synonym, "proc", QueryDeclaration::PROCEDURE);
        PatternClause patternClause2(whileSyn, procSyn, vector<PatternVariable>({patternWildCard}));
        REQUIRE_THROWS(WhilePatternEvaluator(pkbManager).evaluate(patternClause2));

        ClauseVariable readSyn(ClauseVariable::synonym, "read", QueryDeclaration::READ);
        PatternClause patternClause3(whileSyn, readSyn, vector<PatternVariable>({patternWildCard}));
        REQUIRE_THROWS(WhilePatternEvaluator(pkbManager).evaluate(patternClause3));

        PatternClause patternClause4(whileSyn, variableSyn, vector<PatternVariable>({patternFP}));
        REQUIRE_THROWS(WhilePatternEvaluator(pkbManager).evaluate(patternClause4));

        PatternClause patternClause5(whileSyn, variableSyn, vector<PatternVariable>({patternSP}));
    }

    delete pkbManager;
}
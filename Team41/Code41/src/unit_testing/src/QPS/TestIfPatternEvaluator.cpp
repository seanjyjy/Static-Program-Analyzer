#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/IfPatternTable.h"
#include "QPS/Evaluator/IfPatternEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "./TableTestUtils.h"
#include "../Common/AstBuilder/TestAstBuilderUtils.h"

TEST_CASE("Evaluator: If pattern evaluator") {
    PKBManager *pkbManager = new PKBManager();
    string lines[] = {"1", "2", "3", "4"};
    string IF_SYN_LBL = "ifs";
    string VAR_SYN_LBL = "v1";
    string vars[] = {"var0", "var1", "var2"};

    QueryDeclaration ifSyn(new IfEntities(), IF_SYN_LBL);

    ClauseVariable identifierV0(ClauseVariable::identifier, vars[0], new VariableEntities());
    ClauseVariable identifierV1(ClauseVariable::identifier, vars[1], new VariableEntities());
    ClauseVariable variableSyn(ClauseVariable::synonym, VAR_SYN_LBL, new VariableEntities());
    ClauseVariable wildcard(ClauseVariable::wildcard, "_", new VariableEntities());

    PatternVariable patternWildCard(PatternVariable::wildcard, nullptr);
    PatternVariable patternFP(PatternVariable::fullpattern, nullptr);
    PatternVariable patternSP(PatternVariable::subpattern, nullptr);

    pkbManager->registerVariable(vars[0]);
    pkbManager->registerVariable(vars[1]);
    pkbManager->registerVariable(vars[2]);
    pkbManager->registerIfPattern(lines[0], vars[0]); // 0: v0
    pkbManager->registerIfPattern(lines[1], vars[1]); // 1: v1
    pkbManager->registerIfPattern(lines[2], vars[2]); // 2: v2
    pkbManager->registerIfPattern(lines[3], vars[0]); // 3: v0
    pkbManager->registerIfPattern(lines[3], vars[1]); // 3: v1
    pkbManager->registerIfStmt(lines[0]);
    pkbManager->registerIfStmt(lines[1]);
    pkbManager->registerIfStmt(lines[2]);
    pkbManager->registerIfStmt(lines[3]);

    SECTION("Synonym WildCards Pair") {
        PatternClause patternClause1(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{IF_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]},
                                             {lines[1], vars[1]},
                                             {lines[2], vars[2]},
                                             {lines[3], vars[0]},
                                             {lines[3], vars[1]}};

        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Identifier WildCards Pair") {
        PatternClause patternClause1(ifSyn, identifierV0, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{IF_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(ifSyn, identifierV1, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table2 = IfPatternEvaluator(pkbManager).evaluate(patternClause2);
        vector<string> expectedColumns2{IF_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[1]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));

        delete table1;
        delete table2;
    }

    SECTION("WildCard WildCards Pair") {
        PatternClause patternClause1(ifSyn, wildcard, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator(pkbManager).evaluate(patternClause1);
        vector<string> expectedColumns1{IF_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[1]},
                                             {lines[2]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        delete table1;
    }

    SECTION("Semantically & Syntactically Invalid") {
        PatternClause patternClause1(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause1));

        ClauseVariable procSyn(ClauseVariable::synonym, "proc", new ProcedureEntities());
        PatternClause patternClause2(ifSyn, procSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause2));

        ClauseVariable readSyn(ClauseVariable::synonym, "read", new ReadEntities());
        PatternClause patternClause3(ifSyn, readSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause3));

        PatternClause patternClause4(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause4));

        PatternClause patternClause5(ifSyn, variableSyn, vector<PatternVariable>({patternSP, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause5));

        PatternClause patternClause6(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternFP}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause6));

        PatternClause patternClause7(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause7));

        PatternClause patternClause8(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternFP}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause8));

        PatternClause patternClause9(ifSyn, variableSyn, vector<PatternVariable>({patternSP, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause9));

        PatternClause patternClause10(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator(pkbManager).evaluate(patternClause10));
    }

    delete pkbManager;
}
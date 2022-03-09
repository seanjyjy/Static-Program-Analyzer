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

    QueryDeclaration ifSyn(QueryDeclaration::IF, IF_SYN_LBL);

    ClauseVariable identifierV0(ClauseVariable::identifier, vars[0], QueryDeclaration::VARIABLE);
    ClauseVariable identifierV1(ClauseVariable::identifier, vars[1], QueryDeclaration::VARIABLE);
    ClauseVariable variableSyn(ClauseVariable::synonym, VAR_SYN_LBL, QueryDeclaration::VARIABLE);
    ClauseVariable wildcard(ClauseVariable::wildcard, "_", QueryDeclaration::VARIABLE);

    PatternVariable patternWildCard(PatternVariable::wildcard, nullptr);
    PatternVariable patternFP(PatternVariable::fullpattern, nullptr);
    PatternVariable patternSP(PatternVariable::subpattern, nullptr);

    pkbManager->registerIfPattern(lines[0], vars[0]); // 0: v0
    pkbManager->registerIfPattern(lines[1], vars[1]); // 1: v1
    pkbManager->registerIfPattern(lines[2], vars[2]); // 2: v2
    pkbManager->registerIfPattern(lines[3], vars[0]); // 3: v0
    pkbManager->registerIfPattern(lines[3], vars[1]); // 3: v1

    SECTION("Synonym WildCards Pair") {
        PatternClause patternClause1(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator::evaluate(patternClause1, pkbManager);
        vector<string> expectedColumns1{IF_SYN_LBL, VAR_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0], vars[0]},
                                             {lines[1], vars[1]},
                                             {lines[2], vars[2]},
                                             {lines[3], vars[0]},
                                             {lines[3], vars[1]}};

        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));
    }

    SECTION("Identifier WildCards Pair") {
        PatternClause patternClause1(ifSyn, identifierV0, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator::evaluate(patternClause1, pkbManager);
        vector<string> expectedColumns1{IF_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));

        PatternClause patternClause2(ifSyn, identifierV1, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table2 = IfPatternEvaluator::evaluate(patternClause2, pkbManager);
        vector<string> expectedColumns2{IF_SYN_LBL};
        vector<vector<string>> expectedRows2{{lines[1]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table2, expectedColumns2, expectedRows2));
    }

    SECTION("WildCard WildCards Pair") {
        PatternClause patternClause1(ifSyn, wildcard, vector<PatternVariable>({patternWildCard, patternWildCard}));
        Table *table1 = IfPatternEvaluator::evaluate(patternClause1, pkbManager);
        vector<string> expectedColumns1{IF_SYN_LBL};
        vector<vector<string>> expectedRows1{{lines[0]},
                                             {lines[1]},
                                             {lines[2]},
                                             {lines[3]}};
        REQUIRE(TableTestUtils::checkTableMatches(table1, expectedColumns1, expectedRows1));
    }

    SECTION("Semantically & Syntactically Invalid") {
        PatternClause patternClause1(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause1, pkbManager));

        ClauseVariable procSyn(ClauseVariable::synonym, "proc", QueryDeclaration::PROCEDURE);
        PatternClause patternClause2(ifSyn, procSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause2, pkbManager));

        ClauseVariable readSyn(ClauseVariable::synonym, "read", QueryDeclaration::READ);
        PatternClause patternClause3(ifSyn, readSyn, vector<PatternVariable>({patternWildCard, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause3, pkbManager));

        PatternClause patternClause4(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause4, pkbManager));

        PatternClause patternClause5(ifSyn, variableSyn, vector<PatternVariable>({patternSP, patternWildCard}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause5, pkbManager));

        PatternClause patternClause6(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternFP}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause6, pkbManager));

        PatternClause patternClause7(ifSyn, variableSyn, vector<PatternVariable>({patternWildCard, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause7, pkbManager));

        PatternClause patternClause8(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternFP}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause8, pkbManager));

        PatternClause patternClause9(ifSyn, variableSyn, vector<PatternVariable>({patternSP, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause9, pkbManager));

        PatternClause patternClause10(ifSyn, variableSyn, vector<PatternVariable>({patternFP, patternSP}));
        REQUIRE_THROWS(IfPatternEvaluator::evaluate(patternClause10, pkbManager));
    }

    delete pkbManager;
}
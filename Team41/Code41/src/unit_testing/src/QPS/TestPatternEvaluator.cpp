#include "catch.hpp"
#include "PKB/PKB.h"
#include "PKB/PatternTable.h"
#include "QPS/Evaluator/PatternEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "../Common/AstBuilder/TestAstBuilderUtils.h"

TEST_CASE("Evaluator: Pattern evaluator") {
    PKB *pkbManager = new PKB();
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
    pkbManager->registerPattern(lines[0], vars[0], node1);
    // var2 = x+y;
    pkbManager->registerPattern(lines[1], vars[1], node2);
    // var1 = 2;
    pkbManager->registerPattern(lines[2], vars[0], node3);
    // var2 = x+y*z-20;
    pkbManager->registerPattern(lines[3], vars[1], node4);
    for (string l: lines) {
        pkbManager->registerAssign(l);
    }
    pkbManager->registerModifiesS(lines[0], vars[0]);
    pkbManager->registerModifiesS(lines[1], vars[1]);
    pkbManager->registerModifiesS(lines[2], vars[0]);
    pkbManager->registerModifiesS(lines[3], vars[1]);

    SECTION("Synonym FullPattern Pair") {
        // "x+y+z" ?
        PatternClause patternClause1(assignSyn, variableSyn, patternFP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 1);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0]}));
        REQUIRE(table1->getColumn(VAR_SYN_LBL) == unordered_set<string>({vars[0]}));

        // "x+y*z-20" ?
        PatternClause patternClause2(assignSyn, variableSyn, patternFP3);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 1);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[3]}));
        REQUIRE(table2->getColumn(VAR_SYN_LBL) == unordered_set<string>({vars[1]}));
        delete table1;
        delete table2;
    }

    SECTION("Synonym SubPattern Pair") {
        // _x+y_ ?
        PatternClause patternClause1(assignSyn, variableSyn, subpatternSP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 2);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0], lines[1]}));
        REQUIRE(table1->getColumn(VAR_SYN_LBL) == unordered_set<string>({vars[0], vars[1]}));

        // _2_ ?
        PatternClause patternClause2(assignSyn, variableSyn, subpatternSP2);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 1);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[2]}));
        REQUIRE(table2->getColumn(VAR_SYN_LBL) == unordered_set<string>({vars[0]}));
        delete table1;
        delete table2;
    }

    SECTION("Synonym WildCard Pair") {
        PatternClause patternClause2(assignSyn, variableSyn, patternWildCard);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 4);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0], lines[1], lines[2], lines[3]}));
        REQUIRE(table2->getColumn(VAR_SYN_LBL) == unordered_set<string>({vars[0], vars[1]}));
        delete table2;
    }

    SECTION("Identifier FullPattern Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, patternFP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 1);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0]}));

        PatternClause patternClause2(assignSyn, identifierV1, patternFP2);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 0);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>());
        delete table1;
        delete table2;
    }

    SECTION("Identifier SubPattern Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, subpatternSP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 1);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0]}));

        PatternClause patternClause2(assignSyn, identifierV1, subpatternSP2);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 1);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[2]}));
        delete table1;
        delete table2;
    }

    SECTION("Identifier WildCard Pair") {
        PatternClause patternClause1(assignSyn, identifierV1, patternWildCard);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 2);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0], lines[2]}));

        PatternClause patternClause2(assignSyn, identifierV2, patternWildCard);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 2);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[1], lines[3]}));
        delete table1;
        delete table2;
    }

    SECTION("WildCard FullPattern Pair") {
        PatternClause patternClause1(assignSyn, wildcard, patternFP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 1);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0]}));

        PatternClause patternClause2(assignSyn, wildcard, patternFP3);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 1);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[3]}));
        delete table1;
        delete table2;
    }

    SECTION("WildCard SubPattern Pair") {
        PatternClause patternClause1(assignSyn, wildcard, subpatternSP1);
        Table *table1 = PatternEvaluator::evaluate(patternClause1, pkbManager);
        REQUIRE(table1->size() == 2);
        REQUIRE(table1->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0], lines[1]}));

        PatternClause patternClause2(assignSyn, wildcard, subpatternSP2);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 1);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[2]}));
        delete table1;
        delete table2;
    }

    SECTION("WildCard WildCard Pair") {
        PatternClause patternClause2(assignSyn, wildcard, patternWildCard);
        Table *table2 = PatternEvaluator::evaluate(patternClause2, pkbManager);
        REQUIRE(table2->size() == 4);
        REQUIRE(table2->getColumn(ASSIGN_SYN_LBL) == unordered_set<string>({lines[0], lines[1], lines[2], lines[3]}));
        delete table2;
    }

    SECTION("Semantically Invalid") {
        QueryDeclaration stmtSyn(QueryDeclaration::STMT, ASSIGN_SYN_LBL);
        PatternClause patternClause1(stmtSyn, variableSyn, patternFP1);
        REQUIRE(PatternEvaluator::evaluate(patternClause1, pkbManager) == FalseTable::getTable());

        ClauseVariable procSyn(ClauseVariable::synonym, "proc", QueryDeclaration::PROCEDURE);
        PatternClause patternClause2(assignSyn, procSyn, patternFP1);
        REQUIRE(PatternEvaluator::evaluate(patternClause2, pkbManager) == FalseTable::getTable());

        ClauseVariable readSyn(ClauseVariable::synonym, "proc", QueryDeclaration::READ);
        PatternClause patternClause3(assignSyn, readSyn, patternFP1);
        REQUIRE(PatternEvaluator::evaluate(patternClause3, pkbManager) == FalseTable::getTable());
    }

    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete pkbManager;
}
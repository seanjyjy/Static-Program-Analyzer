#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/CallsTable.h"
#include "QPS/Evaluator/WithEvaluator.h"
#include "QPS/WithVariable.h"
#include "./TableTestUtils.h"

TEST_CASE("Evaluator: With evaluator") {
    PKBManager *pkbManager = new PKBManager();

    string STMT_SYN_LBL("stmt1");
    string ASSIGN_SYN_LBL("a1");
    string READ_SYN_LBL("r1");
    string PRINT_SYN_LBL("pn");
    string CALL_SYN_LBL("call");
    string PROC_SYN_LBL("proc");
    string CONST_SYN_LBL("const");
    string VAR_SYN_LBL("var");

    QueryDeclaration stmtSyn(new StmtEntities(), STMT_SYN_LBL);
    QueryDeclaration assignSyn(new AssignEntities(), ASSIGN_SYN_LBL);
    QueryDeclaration readSyn(new ReadEntities(), READ_SYN_LBL);
    QueryDeclaration printSyn(new PrintEntities(), PRINT_SYN_LBL);
    QueryDeclaration callSyn(new CallEntities(), CALL_SYN_LBL);
    QueryDeclaration procSyn(new ProcedureEntities(), PROC_SYN_LBL);
    QueryDeclaration constSyn(new ConstantEntities(), CONST_SYN_LBL);
    QueryDeclaration varSyn(new VariableEntities(), VAR_SYN_LBL);

    WithVariable stmtNoRef(WithVariable::attributeName::STMT_NUM, stmtSyn);
    WithVariable assignStmtNumRef(WithVariable::attributeName::STMT_NUM, assignSyn);
    WithVariable readVarNameRef(WithVariable::attributeName::VAR_NAME, readSyn);
    WithVariable printVarNameRef(WithVariable::attributeName::VAR_NAME, printSyn);
    WithVariable callProcNameRef(WithVariable::attributeName::PROC_NAME, callSyn);
    WithVariable procProcNameRef(WithVariable::attributeName::PROC_NAME, procSyn);
    WithVariable constValRef(WithVariable::attributeName::VALUE, constSyn);
    WithVariable varVarNameRef(WithVariable::attributeName::VAR_NAME, varSyn);

    string proc[] = {"p0", "p1", "p2"};
    string constants[] = {"0", "1", "2"};
    string variable[] = {"c0", "c1"};
    string stmt[] = {"1", "2", "3", "4", "5"};

    for (auto &p: proc) {
        pkbManager->registerProcedure(p);
    }
    for (auto &c: constants) {
        pkbManager->registerConstant(c);
    }
    for (auto &v: variable) {
        pkbManager->registerVariable(v);
    }
    pkbManager->registerAssignStmt(stmt[0]);
    pkbManager->registerReadStmt(stmt[1], variable[0]);
    pkbManager->registerPrintStmt(stmt[2], variable[1]);
    pkbManager->registerCallStmt(stmt[3], proc[0]);
    pkbManager->registerPrintStmt(stmt[4], variable[0]);

    SECTION("Calls Evaluator") {
        SECTION("Integer Integer Pair") {
            WithClause withClause1({"1", WithVariable::INTEGER}, {"2", WithVariable::INTEGER});
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(table1->isFalseTable());

            WithClause withClause2({"1", WithVariable::INTEGER}, {"1", WithVariable::INTEGER});
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(table2->isTrueTable());

            delete table1;
            delete table2;
        }

        SECTION("Integer AttrRef Pair") {
            WithClause withClause1({"1", WithVariable::INTEGER}, stmtNoRef);
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {stmtSyn.getSynonym()}, {{stmt[0]}}));

            WithClause withClause2({"1", WithVariable::INTEGER}, assignStmtNumRef);
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {assignSyn.getSynonym()}, {{stmt[0]}}));

            WithClause withClause3({"0", WithVariable::INTEGER}, constValRef);
            Table *table3 = WithEvaluator(pkbManager).evaluate(withClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {constSyn.getSynonym()}, {{constants[0]}}));

            WithClause withClause4({"99", WithVariable::INTEGER}, constValRef);
            Table *table4 = WithEvaluator(pkbManager).evaluate(withClause4);
            REQUIRE(table4->getRows().empty());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("Identifier Identifier Pair") {
            WithClause withClause1({"1", WithVariable::IDENT}, {"2", WithVariable::IDENT});
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(table1->isFalseTable());

            WithClause withClause2({"1", WithVariable::IDENT}, {"1", WithVariable::IDENT});
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(table2->isTrueTable());

            delete table1;
            delete table2;
        }

        SECTION("Identifier AttrRef Pair") {
            WithClause withClause1({variable[0], WithVariable::IDENT}, readVarNameRef);
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {readSyn.getSynonym()}, {{stmt[1]}}));

            WithClause withClause2({variable[1], WithVariable::IDENT}, printVarNameRef);
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {printSyn.getSynonym()}, {{stmt[2]}}));

            WithClause withClause3({proc[0], WithVariable::IDENT}, callProcNameRef);
            Table *table3 = WithEvaluator(pkbManager).evaluate(withClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {callSyn.getSynonym()}, {{stmt[3]}}));

            WithClause withClause4({proc[1], WithVariable::IDENT}, procProcNameRef);
            Table *table4 = WithEvaluator(pkbManager).evaluate(withClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {procSyn.getSynonym()}, {{proc[1]}}));

            WithClause withClause5({"unknown", WithVariable::IDENT}, procProcNameRef);
            Table *table5 = WithEvaluator(pkbManager).evaluate(withClause5);
            REQUIRE(table5->getRows().empty());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("AttrRef Integer Pair") {
            WithClause withClause1(stmtNoRef, {"1", WithVariable::INTEGER});
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {stmtSyn.getSynonym()}, {{stmt[0]}}));

            WithClause withClause2(assignStmtNumRef, {"1", WithVariable::INTEGER});
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {assignSyn.getSynonym()}, {{stmt[0]}}));

            WithClause withClause3(constValRef, {"0", WithVariable::INTEGER});
            Table *table3 = WithEvaluator(pkbManager).evaluate(withClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {constSyn.getSynonym()}, {{constants[0]}}));

            WithClause withClause4(constValRef, {"99", WithVariable::INTEGER});
            Table *table4 = WithEvaluator(pkbManager).evaluate(withClause4);
            REQUIRE(table4->getRows().empty());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
        }

        SECTION("AttrRef Identifier Pair") {
            WithClause withClause1(readVarNameRef, {variable[0], WithVariable::IDENT});
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1, {readSyn.getSynonym()}, {{stmt[1]}}));

            WithClause withClause2(printVarNameRef, {variable[1], WithVariable::IDENT});
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2, {printSyn.getSynonym()}, {{stmt[2]}}));

            WithClause withClause3(callProcNameRef, {proc[0], WithVariable::IDENT});
            Table *table3 = WithEvaluator(pkbManager).evaluate(withClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3, {callSyn.getSynonym()}, {{stmt[3]}}));

            WithClause withClause4(procProcNameRef, {proc[1], WithVariable::IDENT});
            Table *table4 = WithEvaluator(pkbManager).evaluate(withClause4);
            REQUIRE(TableTestUtils::checkTableMatches(table4, {procSyn.getSynonym()}, {{proc[1]}}));

            WithClause withClause5(procProcNameRef, {"unknown", WithVariable::IDENT});
            Table *table5 = WithEvaluator(pkbManager).evaluate(withClause5);
            REQUIRE(table5->getRows().empty());

            delete table1;
            delete table2;
            delete table3;
            delete table4;
            delete table5;
        }

        SECTION("AttrRef AttrRef Pair") {
            WithClause withClause1(readVarNameRef, printVarNameRef);
            Table *table1 = WithEvaluator(pkbManager).evaluate(withClause1);
            REQUIRE(TableTestUtils::checkTableMatches(table1,
                                                      {readSyn.getSynonym(), printSyn.getSynonym()},
                                                      {{stmt[1], stmt[4]}}));

            WithClause withClause2(printVarNameRef, varVarNameRef);
            Table *table2 = WithEvaluator(pkbManager).evaluate(withClause2);
            REQUIRE(TableTestUtils::checkTableMatches(table2,
                                                      {printSyn.getSynonym(), varSyn.getSynonym()},
                                                      {{stmt[2], variable[1]}, {stmt[4], variable[0]}}));

            WithClause withClause3(callProcNameRef, procProcNameRef);
            Table *table3 = WithEvaluator(pkbManager).evaluate(withClause3);
            REQUIRE(TableTestUtils::checkTableMatches(table3,
                                                      {callSyn.getSynonym(), procSyn.getSynonym()},
                                                      {{stmt[3], proc[0]}}));

            WithClause withClause4(procProcNameRef, varVarNameRef);
            Table *table4 = WithEvaluator(pkbManager).evaluate(withClause4);
            REQUIRE(table4->getRows().empty());

            WithClause withClause5(stmtNoRef, assignStmtNumRef);
            Table *table5 = WithEvaluator(pkbManager).evaluate(withClause5);
            REQUIRE(TableTestUtils::checkTableMatches(table5,
                                                      {stmtSyn.getSynonym(), assignSyn.getSynonym()},
                                                      {{stmt[0], stmt[0]}}));

            WithClause withClause6(constValRef, stmtNoRef);
            Table *table6 = WithEvaluator(pkbManager).evaluate(withClause6);
            REQUIRE(TableTestUtils::checkTableMatches(table6,
                                                      {constSyn.getSynonym(), stmtSyn.getSynonym()},
                                                      {{constants[1], stmt[0]}, {constants[2], stmt[1]}}));
        }

        SECTION("Mismatch type") {
            WithClause withClause1({"1", WithVariable::INTEGER}, {"2", WithVariable::IDENT});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause1));

            WithClause withClause2({"1", WithVariable::IDENT}, {"2", WithVariable::INTEGER});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause2));

            WithClause withClause3(readVarNameRef, {"2", WithVariable::INTEGER});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause3));

            WithClause withClause4({"1", WithVariable::INTEGER}, readVarNameRef);
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause4));

            WithClause withClause5(constValRef, {"2", WithVariable::IDENT});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause5));

            WithClause withClause6({"1", WithVariable::IDENT}, constValRef);
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(withClause6));
        }
    }
    delete pkbManager;
}
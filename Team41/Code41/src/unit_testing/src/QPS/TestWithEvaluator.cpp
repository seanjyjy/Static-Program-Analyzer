#include <stack>

#include "catch.hpp"
#include "PKB/PKBManager.h"
#include "PKB/Tables/CallsTable.h"
#include "QPS/Evaluator/WithEvaluator.h"
#include "QPS/QO/WithVariable.h"
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

    StmtEntities stmtEntities;
    AssignEntities assignEntities;
    ReadEntities readEntities;
    PrintEntities printEntities;
    CallEntities callEntities;
    ProcedureEntities procedureEntities;
    ConstantEntities constantEntities;
    VariableEntities variableEntities;

    QueryDeclaration stmtSyn(&stmtEntities, STMT_SYN_LBL);
    QueryDeclaration assignSyn(&assignEntities, ASSIGN_SYN_LBL);
    QueryDeclaration readSyn(&readEntities, READ_SYN_LBL);
    QueryDeclaration printSyn(&printEntities, PRINT_SYN_LBL);
    QueryDeclaration callSyn(&callEntities, CALL_SYN_LBL);
    QueryDeclaration procSyn(&procedureEntities, PROC_SYN_LBL);
    QueryDeclaration constSyn(&constantEntities, CONST_SYN_LBL);
    QueryDeclaration varSyn(&variableEntities, VAR_SYN_LBL);

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

    stack<WithClause> clauses;
    stack<Table *> tables;

    SECTION("Calls Evaluator") {
        SECTION("Integer Integer Pair") {
            clauses.push({{"1", WithVariable::INTEGER},
                          {"2", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({{"1", WithVariable::INTEGER},
                          {"1", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

        }

        SECTION("Integer AttrRef Pair") {
            clauses.push({{"1", WithVariable::INTEGER}, stmtNoRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {stmtSyn.getSynonym()}, {{stmt[0]}}));

            clauses.push({{"1", WithVariable::INTEGER}, assignStmtNumRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {assignSyn.getSynonym()}, {{stmt[0]}}));

            clauses.push({{"0", WithVariable::INTEGER}, constValRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {constSyn.getSynonym()}, {{constants[0]}}));

            clauses.push({{"99", WithVariable::INTEGER}, constValRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->getRows().empty());

        }

        SECTION("Identifier Identifier Pair") {
            clauses.push({{"1", WithVariable::IDENT}, {"2", WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({{"1", WithVariable::IDENT}, {"1", WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

        }

        SECTION("Identifier AttrRef Pair") {
            clauses.push({{variable[0], WithVariable::IDENT}, readVarNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {readSyn.getSynonym()}, {{stmt[1]}}));

            clauses.push({{variable[1], WithVariable::IDENT}, printVarNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {printSyn.getSynonym()}, {{stmt[2]}}));

            clauses.push({{proc[0], WithVariable::IDENT}, callProcNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {callSyn.getSynonym()}, {{stmt[3]}}));

            clauses.push({{proc[1], WithVariable::IDENT}, procProcNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {procSyn.getSynonym()}, {{proc[1]}}));

            clauses.push({{"unknown", WithVariable::IDENT}, procProcNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->getRows().empty());

        }

        SECTION("AttrRef Integer Pair") {
            clauses.push({stmtNoRef, {"1", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {stmtSyn.getSynonym()}, {{stmt[0]}}));

            clauses.push({assignStmtNumRef, {"1", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {assignSyn.getSynonym()}, {{stmt[0]}}));

            clauses.push({constValRef, {"0", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {constSyn.getSynonym()}, {{constants[0]}}));

            clauses.push({constValRef, {"99", WithVariable::INTEGER}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->getRows().empty());

        }

        SECTION("AttrRef Identifier Pair") {
            clauses.push({readVarNameRef, {variable[0], WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {readSyn.getSynonym()}, {{stmt[1]}}));

            clauses.push({printVarNameRef, {variable[1], WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {printSyn.getSynonym()}, {{stmt[2]}}));

            clauses.push({callProcNameRef, {proc[0], WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {callSyn.getSynonym()}, {{stmt[3]}}));

            clauses.push({procProcNameRef, {proc[1], WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {procSyn.getSynonym()}, {{proc[1]}}));

            clauses.push({procProcNameRef, {"unknown", WithVariable::IDENT}});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->getRows().empty());

        }

        SECTION("AttrRef AttrRef Pair") {
            clauses.push({readVarNameRef, printVarNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {readSyn.getSynonym(), printSyn.getSynonym()},
                                                      {{stmt[1], stmt[4]}}));

            clauses.push({printVarNameRef, varVarNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {printSyn.getSynonym(), varSyn.getSynonym()},
                                                      {{stmt[2], variable[1]}, {stmt[4], variable[0]}}));

            clauses.push({callProcNameRef, procProcNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {callSyn.getSynonym(), procSyn.getSynonym()},
                                                      {{stmt[3], proc[0]}}));

            clauses.push({procProcNameRef, varVarNameRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(tables.top()->getRows().empty());

            clauses.push({stmtNoRef, assignStmtNumRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {stmtSyn.getSynonym(), assignSyn.getSynonym()},
                                                      {{stmt[0], stmt[0]}}));

            clauses.push({constValRef, stmtNoRef});
            tables.push(WithEvaluator(pkbManager).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {constSyn.getSynonym(), stmtSyn.getSynonym()},
                                                      {{constants[1], stmt[0]}, {constants[2], stmt[1]}}));

        }

        SECTION("Mismatch type") {
            clauses.push({{"1", WithVariable::INTEGER}, {"2", WithVariable::IDENT}});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));

            clauses.push({{"1", WithVariable::IDENT}, {"2", WithVariable::INTEGER}});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));

            clauses.push({readVarNameRef, {"2", WithVariable::INTEGER}});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));

            clauses.push({{"1", WithVariable::INTEGER}, readVarNameRef});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));

            clauses.push({constValRef, {"2", WithVariable::IDENT}});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));

            clauses.push({{"1", WithVariable::IDENT}, constValRef});
            REQUIRE_THROWS(WithEvaluator(pkbManager).evaluate(clauses.top()));
        }
    }
    while (!tables.empty()) {
        delete tables.top();
        tables.pop();
    }
    delete pkbManager;
}
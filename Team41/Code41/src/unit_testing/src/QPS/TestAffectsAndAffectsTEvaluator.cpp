#include <stack>

#include "catch.hpp"
#include "Common/CFGBuilder.h"
#include "PKB/PKBManager.h"
#include "QPS/Evaluator/AffectsEvaluator.h"
#include "QPS/Evaluator/AffectsTEvaluator.h"
#include "QPS/ClauseVariable.h"
#include "./TableTestUtils.h"

TEST_CASE("Evaluator: Affects and AffectsT evaluator") {
    auto *pkbManager = new PKBManager();
    auto *affectsKbAdapter = new AffectsKBAdapter(pkbManager);

    /**
     *    procedure p {
     * 1.    x = 1;
     * 2.    y = 2;
     * 3.    z = y;
     * 4.    call q;
     * 5.    z = x + y + z;
     * 6.    while(true) {
     * 7.       z = z + 1; }}
     *    procedure q {
     * 8.    x = 5;
     * 9.    t = 4;
     * 10.    if ( z > 0 ) then {
     * 11.        t = x + 1;
     *       } else {
     * 12.       y = z + x; }
     * 13.  x = t + 1; }
     */

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3"}},
            {"3",  {"4"}},
            {"4",  {"5"}},
            {"5",  {"6"}},
            {"6",  {"7"}},
            {"7",  {"6"}},
            {"8",  {"9"}},
            {"9",  {"10"}},
            {"10", {"11", "12"}},
            {"11", {"13"}},
            {"12", {"13"}},
            {"13", {}},
    };
    CFGBuilder nextCfgBuilder = CFGBuilder(stmtNextMap, {"1", "8"});
    nextCfgBuilder.build();
    CFGNode *nextRootNode = nextCfgBuilder.getCFG();
    unordered_map<string, CFGNode *> stmtToNextCFG = nextCfgBuilder.getStmtNumToNodeMap();
    pkbManager->registerCFG(nextRootNode, stmtToNextCFG);

    pkbManager->registerAssignStmt("8");
    TableTestUtils::registerUsesModify(pkbManager, "q", "8", {"x"}, {});
    pkbManager->registerAssignStmt("9");
    TableTestUtils::registerUsesModify(pkbManager, "q", "9", {"t"}, {});
    pkbManager->registerIfStmt("10");
    TableTestUtils::registerUsesModify(pkbManager, "q", "10", {"t", "y"}, {"x", "z", "z"});
    pkbManager->registerAssignStmt("11");
    TableTestUtils::registerUsesModify(pkbManager, "q", "11", {"t"}, {"x"});
    pkbManager->registerAssignStmt("12");
    TableTestUtils::registerUsesModify(pkbManager, "q", "12", {"y"}, {"z", "x"});
    pkbManager->registerAssignStmt("13");
    TableTestUtils::registerUsesModify(pkbManager, "q", "13", {"x"}, {"t"});

    pkbManager->registerAssignStmt("1");
    TableTestUtils::registerUsesModify(pkbManager, "p", "1", {"x"}, {});
    pkbManager->registerAssignStmt("2");
    TableTestUtils::registerUsesModify(pkbManager, "p", "2", {"y"}, {});
    pkbManager->registerAssignStmt("3");
    TableTestUtils::registerUsesModify(pkbManager, "p", "3", {"z"}, {"y"});
    TableTestUtils::registerCalls(pkbManager, "4", "q");
    pkbManager->registerAssignStmt("5");
    TableTestUtils::registerUsesModify(pkbManager, "p", "5", {"z"}, {"x", "y", "z"});
    pkbManager->registerWhileStmt("6");
    TableTestUtils::registerUsesModify(pkbManager, "p", "6", {"z"}, {"z"});
    pkbManager->registerAssignStmt("7");
    TableTestUtils::registerUsesModify(pkbManager, "p", "7", {"z"}, {"z"});

    vector<ClauseVariable> integers;
    for (int i = 0; i <= 13; ++i) {
        integers.push_back({ClauseVariable::variable_type::integer, std::to_string(i), new ConstantEntities()});
    }

    ClauseVariable synonymAssign1(ClauseVariable::variable_type::synonym, "a1", new AssignEntities());
    ClauseVariable synonymAssign2(ClauseVariable::variable_type::synonym, "a2", new AssignEntities());
    ClauseVariable synonymStmt1(ClauseVariable::variable_type::synonym, "s1", new StmtEntities());
    ClauseVariable synonymStmt2(ClauseVariable::variable_type::synonym, "s2", new StmtEntities());

    ClauseVariable wildcard(ClauseVariable::variable_type::wildcard, "_", new NoneEntities());

    stack<QueryClause> clauses;
    stack<Table *> tables;

    SECTION("Affects Evaluator") {
        SECTION("Integer Integer pair") {
            clauses.push({QueryClause::affects, integers[2], integers[3]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, integers[3], integers[5]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, integers[3], integers[7]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, integers[7], integers[7]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, integers[5], integers[7]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, integers[9], integers[13]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, integers[1], integers[5]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // invalid stmtNumber
            clauses.push({QueryClause::affects, integers[0], integers[1]});
            REQUIRE(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top())->isFalseTable());
        }

        SECTION("Integer Synonym pair") {
//             * 1.    x = 1;
            clauses.push({QueryClause::affects, integers[1], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

//             * 2.    y = 2;
            clauses.push({QueryClause::affects, integers[2], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"3"}}));

//             * 3.    z = y;
            clauses.push({QueryClause::affects, integers[3], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"5"}}));

//             * 5.    z = x + y + z;
            clauses.push({QueryClause::affects, integers[5], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"7"}}));

//             * 7.       z = z + 1; }}
            clauses.push({QueryClause::affects, integers[7], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"7"}}));

//             * 8.    x = 5;
            clauses.push({QueryClause::affects, integers[8], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"11"},
                                                                                                {"12"}}));

//             * 9.    t = 4;
            clauses.push({QueryClause::affects, integers[9], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"13"}}));

//             * 11.        t = x + 1;
            clauses.push({QueryClause::affects, integers[11], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"13"}}));

//             * 12.       y = z + x; }
            clauses.push({QueryClause::affects, integers[12], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

//             * 13.  x = t + 1; }
            clauses.push({QueryClause::affects, integers[13], synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());
        }

        SECTION("Integer Wildcard pair") {
//             * 1.    x = 1;
            clauses.push({QueryClause::affects, integers[1], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

//             * 2.    y = 2;
            clauses.push({QueryClause::affects, integers[2], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 3.    z = y;
            clauses.push({QueryClause::affects, integers[3], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 5.    z = x + y + z;
            clauses.push({QueryClause::affects, integers[5], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 7.       z = z + 1; }}
            clauses.push({QueryClause::affects, integers[7], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 8.    x = 5;
            clauses.push({QueryClause::affects, integers[8], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 9.    t = 4;
            clauses.push({QueryClause::affects, integers[9], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 11.        t = x + 1;
            clauses.push({QueryClause::affects, integers[11], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

//             * 12.       y = z + x; }
            clauses.push({QueryClause::affects, integers[12], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

//             * 13.  x = t + 1; }
            clauses.push({QueryClause::affects, integers[13], wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());
        }

        SECTION("Synonym Integer pair") {
            clauses.push({QueryClause::affects, synonymStmt1, integers[1]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, synonymStmt1, integers[2]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, synonymStmt1, integers[3]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"}}));

            clauses.push({QueryClause::affects, synonymStmt1, integers[5]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"3"}}));

            clauses.push({QueryClause::affects, synonymStmt1, integers[7]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"5"},
                                                                                                {"7"}}));

            clauses.push({QueryClause::affects, synonymStmt1, integers[8]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, synonymStmt1, integers[9]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, synonymStmt1, integers[11]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"8"}}));

            clauses.push({QueryClause::affects, synonymStmt1, integers[12]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"8"}}));

            clauses.push({QueryClause::affects, synonymStmt1, integers[13]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"9"},
                                                                                                {"11"}}));
        }

        SECTION("Synonym Synonym pair") {
            vector<vector<string>> rows({{"2",  "3"},
                                         {"3",  "5"},
                                         {"5",  "7"},
                                         {"7",  "7"},
                                         {"8",  "11"},
                                         {"8",  "12"},
                                         {"9",  "13"},
                                         {"11", "13"}});

            clauses.push({QueryClause::affects, synonymStmt1, synonymStmt2});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel(), synonymStmt2.getLabel()},
                                                      rows));

            clauses.push({QueryClause::affects, synonymAssign1, synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {synonymAssign1.getLabel(), synonymStmt1.getLabel()}, rows));

        }

        SECTION("Synonym Wildcard pair") {
            clauses.push({QueryClause::affects, synonymStmt1, wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"},
                                                                                                {"3"},
                                                                                                {"5"},
                                                                                                {"7"},
                                                                                                {"8"},
                                                                                                {"9"},
                                                                                                {"11"}}));
        }

        SECTION("Wildcard Integer pair") {
            clauses.push({QueryClause::affects, wildcard, integers[1]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, wildcard, integers[2]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, wildcard, integers[3]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, wildcard, integers[5]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, wildcard, integers[7]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, wildcard, integers[8]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, wildcard, integers[9]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affects, wildcard, integers[11]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, wildcard, integers[12]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affects, wildcard, integers[13]});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());
        }

        SECTION("Wildcard Synonym pair") {
            clauses.push({QueryClause::affects, wildcard, synonymStmt1});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"3"},
                                                                                                {"5"},
                                                                                                {"7"},
                                                                                                {"11"},
                                                                                                {"12"},
                                                                                                {"13"}}));

        }

        SECTION("Wildcard Wildcard pair") {
            clauses.push({QueryClause::affects, wildcard, wildcard});
            tables.push(AffectsEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

        }
    }

    SECTION("AffectsT Evaluator") {
        SECTION("Integer Integer pair") {
            clauses.push({QueryClause::affectsT, integers[2], integers[5]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[3], integers[5]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[3], integers[7]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[7], integers[7]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[8], integers[13]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[9], integers[13]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, integers[1], integers[5]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // invalid stmtNumber
            clauses.push({QueryClause::affectsT, integers[0], integers[13]});
            REQUIRE(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top())->isFalseTable());
        }

        SECTION("Integer Synonym pair") {
            // 1.    x = 1;
            clauses.push({QueryClause::affectsT, integers[1], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // 2.    y = 2;
            clauses.push({QueryClause::affectsT, integers[2], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"3"},
                                                                                                {"5"},
                                                                                                {"7"}}));

            // 3.    z = y;
            clauses.push({QueryClause::affectsT, integers[3], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"5"},
                                                                                                {"7"}}));

            // 5.    z = x + y + z;
            clauses.push({QueryClause::affectsT, integers[5], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"7"}}));

            // 7.       z = z + 1; }}
            clauses.push({QueryClause::affectsT, integers[7], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"7"}}));

            // 8.    x = 5;
            clauses.push({QueryClause::affectsT, integers[8], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"11"},
                                                                                                {"12"},
                                                                                                {"13"}}));

            // 9.    t = 4;
            clauses.push({QueryClause::affectsT, integers[9], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"13"}}));

            // 11.        t = x + 1;
            clauses.push({QueryClause::affectsT, integers[11], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"13"}}));

            // 12.       y = z + x; }
            clauses.push({QueryClause::affectsT, integers[12], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // 13.  x = t + 1; }
            clauses.push({QueryClause::affectsT, integers[13], synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());
        }

        SECTION("Integer Wildcard pair") {
            // 1.    x = 1;
            clauses.push({QueryClause::affectsT, integers[1], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // 2.    y = 2;
            clauses.push({QueryClause::affectsT, integers[2], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 3.    z = y;
            clauses.push({QueryClause::affectsT, integers[3], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 5.    z = x + y + z;
            clauses.push({QueryClause::affectsT, integers[5], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 7.       z = z + 1; }}
            clauses.push({QueryClause::affectsT, integers[7], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 8.    x = 5;
            clauses.push({QueryClause::affectsT, integers[8], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 9.    t = 4;
            clauses.push({QueryClause::affectsT, integers[9], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 11.        t = x + 1;
            clauses.push({QueryClause::affectsT, integers[11], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            // 12.       y = z + x; }
            clauses.push({QueryClause::affectsT, integers[12], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            // 13.  x = t + 1; }
            clauses.push({QueryClause::affectsT, integers[13], wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());
        }

        SECTION("Synonym Integer pair") {
            clauses.push({QueryClause::affectsT, synonymStmt1, integers[1]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[2]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[3]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"}}));

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[5]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"},
                                                                                                {"3"}}));

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[7]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"},
                                                                                                {"3"},
                                                                                                {"5"},
                                                                                                {"7"}}));

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[8]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[9]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[11]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"8"}}));

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[12]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"8"}}));

            clauses.push({QueryClause::affectsT, synonymStmt1, integers[13]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"8"},
                                                                                                {"9"},
                                                                                                {"11"}}));
        }

        SECTION("Synonym Synonym pair") {
            vector<vector<string>> rows({{"2",  "3"},
                                         {"2",  "5"},
                                         {"2",  "7"},
                                         {"3",  "5"},
                                         {"3",  "7"},
                                         {"5",  "7"},
                                         {"7",  "7"},
                                         {"8",  "11"},
                                         {"8",  "12"},
                                         {"8",  "13"},
                                         {"9",  "13"},
                                         {"11", "13"}});

            clauses.push({QueryClause::affectsT, synonymStmt1, synonymStmt2});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel(), synonymStmt2.getLabel()},
                                                      rows));

            clauses.push({QueryClause::affectsT, synonymAssign1, synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(),
                                                      {synonymAssign1.getLabel(), synonymStmt1.getLabel()}, rows));
        }

        SECTION("Synonym Wildcard pair") {
            clauses.push({QueryClause::affectsT, synonymStmt1, wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"2"},
                                                                                                {"3"},
                                                                                                {"5"},
                                                                                                {"7"},
                                                                                                {"8"},
                                                                                                {"9"},
                                                                                                {"11"}}));
        }

        SECTION("Wildcard Integer pair") {
            clauses.push({QueryClause::affectsT, wildcard, integers[1]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[2]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[3]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[5]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[7]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[8]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[9]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isFalseTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[11]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[12]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

            clauses.push({QueryClause::affectsT, wildcard, integers[13]});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());
        }

        SECTION("Wildcard Synonym pair") {
            clauses.push({QueryClause::affectsT, wildcard, synonymStmt1});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(TableTestUtils::checkTableMatches(tables.top(), {synonymStmt1.getLabel()}, {{"3"},
                                                                                                {"5"},
                                                                                                {"7"},
                                                                                                {"11"},
                                                                                                {"12"},
                                                                                                {"13"}}));

        }

        SECTION("Wildcard Wildcard pair") {
            clauses.push({QueryClause::affectsT, wildcard, wildcard});
            tables.push(AffectsTEvaluator(pkbManager, affectsKbAdapter).evaluate(clauses.top()));
            REQUIRE(tables.top()->isTrueTable());

        }
    }

    while (!tables.empty()) {
        delete tables.top();
        tables.pop();
    }
    delete affectsKbAdapter;
    delete pkbManager;
}

#include "catch.hpp"
#include "PKB/PatternTable.h"
#include "../UnitTestUtility.h"

using namespace std;

TEST_CASE("PatternTable") {
    PatternTable table;
    string stmt[] = {"s0", "s1", "s2"};
    string vars[] = {"v0", "v1", "v2"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    Token varTok = Token(TokenType::name, "v0", {0, 0}, {0, 0});
    Token oneTok = Token(TokenType::integer, "1", {0, 0}, {0, 0});
    Token twoTok = Token(TokenType::integer, "2", {0, 0}, {0, 0});
    Token threeTok = Token(TokenType::integer, "3", {0, 0}, {0, 0});
    Token fourTok = Token(TokenType::integer, "4", {0, 0}, {0, 0});

    // 1 + 2 * 3 - 1 / 3 + v0
    TNode *varNode = TNode::makeVarName(&varTok);
    TNode *one = TNode::makeConstVal(&oneTok);
    TNode *two = TNode::makeConstVal(&twoTok);
    TNode *three = TNode::makeConstVal(&threeTok);
    TNode *four = TNode::makeConstVal(&fourTok);
    TNode *times = TNode::makeTimes(two, three);
    TNode *divide = TNode::makeDiv(one, three);
    TNode *plus = TNode::makePlus(one, times);
    TNode *minus = TNode::makeMinus(plus, divide);
    TNode *plusVar = TNode::makePlus(minus, varNode);

    SECTION("FullPattern") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getStmtNVarFromFullPattern(plusVar), EMPTY_SET_PAIR));
            REQUIRE(table.getAllStmtsFromFullPattern(plusVar) == unordered_set<string>());
            REQUIRE(table.getStmtFromFullPatternNVar(plusVar, vars[0]) == unordered_set<string>());
        }

        SECTION("Basic functionality") {
            REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[0], plusVar));
            REQUIRE(sortAndCompareVectors(table.getStmtNVarFromFullPattern(plusVar),
                                          vector<pair<string, string>>({{stmt[0], vars[0]}})));
            REQUIRE(table.getAllStmtsFromFullPattern(plusVar) == unordered_set<string>({stmt[0]}));
            REQUIRE(table.getStmtFromFullPatternNVar(plusVar, vars[0]) == unordered_set<string>({stmt[0]}));
            REQUIRE(table.getStmtFromFullPatternNVar(plusVar, vars[1]) == unordered_set<string>());

            REQUIRE_NOTHROW(table.setPattern(stmt[1], vars[1], plusVar));
            REQUIRE(sortAndCompareVectors(table.getStmtNVarFromFullPattern(plusVar),
                                          vector<pair<string, string>>({{stmt[0], vars[0]},
                                                                        {stmt[1], vars[1]}})));
            REQUIRE(table.getAllStmtsFromFullPattern(plusVar) == unordered_set<string>({stmt[0], stmt[1]}));
            REQUIRE(table.getStmtFromFullPatternNVar(plusVar, vars[0]) == unordered_set<string>({stmt[0]}));
            REQUIRE(table.getStmtFromFullPatternNVar(plusVar, vars[1]) == unordered_set<string>({stmt[1]}));

            // empty result
            REQUIRE(table.getAllStmtsFromFullPattern(plus) == unordered_set<string>({}));
            REQUIRE(table.getAllStmtsFromFullPattern(varNode) == unordered_set<string>({}));
        }
    }

    SECTION("SubPattern") {
        SECTION("Initial State") {
            REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(plusVar), EMPTY_SET_PAIR));
            REQUIRE(table.getAllStmtsFromSubPattern(plusVar) == unordered_set<string>());
            REQUIRE(table.getStmtFromSubPatternNVar(plusVar, vars[0]) == unordered_set<string>());
        }

        SECTION("Basic functionality") {
            REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[0], plusVar));
            SECTION("One unique pattern") {
                REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[0], plusVar));
                for (TNode *child: {one, two, three, varNode, plusVar, minus, plus, divide, times}) {
                    REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(child),
                                                  vector<pair<string, string>>({{stmt[0], vars[0]}})));
                    REQUIRE(table.getAllStmtsFromSubPattern(child) == unordered_set<string>({stmt[0]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[0]) == unordered_set<string>({stmt[0]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[1]) == unordered_set<string>());
                }
            }

            SECTION("Overlapping Pattern") {
                // v0 = 1 + 2 * 3 - 1 / 3 + v0
                REQUIRE_NOTHROW(table.setPattern(stmt[0], vars[0], plusVar));
                // v1 = 2 * 3 - 1 / 3
                TNode* timesMinusDivide = TNode::makeMinus(times, divide);
                REQUIRE_NOTHROW(table.setPattern(stmt[1], vars[1], timesMinusDivide));

                // with both statements
                for (TNode *child: {one, two, three, divide, times}) {
                    REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(child),
                                                  vector<pair<string, string>>({{stmt[0], vars[0]}, {stmt[1], vars[1]}})));
                    REQUIRE(table.getAllStmtsFromSubPattern(child) == unordered_set<string>({stmt[0], stmt[1]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[0]) == unordered_set<string>({stmt[0]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[1]) == unordered_set<string>({stmt[1]}));
                }

                // unique to stmt[0]
                for (TNode *child: {varNode, plusVar, minus, plus}) {
                    REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(child),
                                                  vector<pair<string, string>>({{stmt[0], vars[0]}})));
                    REQUIRE(table.getAllStmtsFromSubPattern(child) == unordered_set<string>({stmt[0]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[0]) == unordered_set<string>({stmt[0]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[1]) == unordered_set<string>());
                }

                // unique to stmt[1]
                for (TNode *child: { timesMinusDivide }) {
                    REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(child),
                                                  vector<pair<string, string>>({{stmt[1], vars[1]}})));
                    REQUIRE(table.getAllStmtsFromSubPattern(child) == unordered_set<string>({stmt[1]}));
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[0]) == unordered_set<string>());
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[1]) == unordered_set<string>({stmt[1]}));
                }

                // unique to none
                for (TNode *child: { four }) {
                    REQUIRE(sortAndCompareVectors(table.getStmtNVarFromSubPattern(child),
                                                  vector<pair<string, string>>()));
                    REQUIRE(table.getAllStmtsFromSubPattern(child) == unordered_set<string>());
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[0]) == unordered_set<string>());
                    REQUIRE(table.getStmtFromSubPatternNVar(child, vars[1]) == unordered_set<string>());
                }
            }
        }
    }
}

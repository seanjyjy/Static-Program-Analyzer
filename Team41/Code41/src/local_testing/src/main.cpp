#include "SimpleParser/Parser.h"
#include "SimpleParser/SPUtils.h"
#include <Exception/SemanticException.h>
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "PKB/Tables/AssignPatternTable.h"
#include "../../unit_testing/src/Common/AstNode/AstUtils.h"

using namespace std;

#include <iostream>

int main() {
    AssignPatternTable table;
    string stmt[] = {"s0", "s1", "s2"};
    string vars[] = {"v0", "v1", "v2"};
    unordered_set<string> EMPTY_SET;
    vector<pair<string, string>> EMPTY_SET_PAIR;

    Token *varTok = Token::makeName("v0");
    Token *oneTok = Token::makeConst("1");
    Token *oneTok_copy = oneTok->copy();
    Token *twoTok = Token::makeConst("2");
    Token *threeTok = Token::makeConst("3");
    Token *threeTok_copy = threeTok->copy();
    Token *fourTok = Token::makeConst("4");

    // 1 + 2 * 3 - 1 / 3 + v0
    VarName *varNode = AstUtils::makeVarName(varTok);
    ConstVal *one = AstUtils::makeConstVal(oneTok);
    ConstVal *one_copy = AstUtils::makeConstVal(oneTok_copy);
    ConstVal *two = AstUtils::makeConstVal(twoTok);
    ConstVal *three = AstUtils::makeConstVal(threeTok);
    ConstVal *three_copy = AstUtils::makeConstVal(threeTok_copy);
    ConstVal *four = AstUtils::makeConstVal(fourTok);
    Times *times = AstUtils::makeTimes(two, three);
    Div *divide = AstUtils::makeDiv(one, three_copy);
    Plus *plus = AstUtils::makePlus(one_copy, times);
    Minus *minus = AstUtils::makeMinus(plus, divide);
    TNode *plusVar = AstUtils::makePlus(minus, varNode);
    AstUtils::printAst(plusVar);
}
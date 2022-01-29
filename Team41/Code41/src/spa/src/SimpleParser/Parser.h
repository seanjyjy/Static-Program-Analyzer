#pragma once

#include "Tokens.h"
#include "Common/TNode.h"

using namespace std;

class Parser {
private:
    Tokens tokens;
    int cursor;
    Token currToken;

    void advance();
    int saveCursor();
    void backtrack(int to);

    TNode &eatProgram();
    TNode &eatProcedure();
    TNode &eatStmtLst();
    TNode &eatStmt();
    TNode &eatStmtRead();
    TNode &eatStmtPrint();
    TNode &eatStmtCall();
    TNode &eatStmtWhile();
    TNode &eatStmtIf();
    TNode &eatStmtAssign();
    TNode &eatCondExpr();
    TNode &eatRelExpr();
    TNode &eatRelFactor();
    TNode &eatExpr();
    TNode &eatExpr1();
    TNode &eatTerm();
    TNode &eatTerm1();
    TNode &eatFactor();
    TNode &eatVarName();
    TNode &eatProcName();
    TNode &eatConstVal();
public:
    Parser();
    TNode parse(string &input);
};
#pragma once

#include "Tokens.h"
#include "Common/TNode.h"

using namespace std;

class Parser {
private:
    enum class Option {
        program,
        procedure,
        stmtlist,
        stmt,
        readStmt,
        printStmt,
        callStmt,
        whileStmt,
        ifStmt,
        assignStmt,
        condExpr,
        expr,
        term,
        name,
        constant,
    };

    string input;
    int cursor;
    Tokens tokens;
    Token currToken;
    Option parseOption = Option::program;

    int saveCursor();

    void backtrack(int to);

    void advance();

    void expect(TokenType type);

    void expect(TokenType type, string val);

    Token *checkAndAdvance(TokenType type);

    Token *checkAndAdvance(TokenType type, string val);

    bool peekMatchType(TokenType type);

    bool peekMatchTypeVal(TokenType type, string val);

    bool isEof();

    string genErrorMsgWithCurrToken();

    string genSyntaxErrorMsg();

    TNode *eatProgram();

    TNode *eatProcedure();

    TNode *eatStmtLst();

    TNode *eatStmt();

    TNode *eatStmtRead();

    TNode *eatStmtPrint();

    TNode *eatStmtCall();

    TNode *eatStmtWhile();

    TNode *eatStmtIf();

    TNode *eatStmtAssign();

    TNode *eatCondExpr();

    TNode *eatRelExpr();

    TNode *eatRelFactor();

    TNode *eatExpr();

    TNode *eatExpr1();

    TNode *eatTerm();

    TNode *eatTerm1();

    TNode *eatFactor();

    void init(string &s);

    TNode *parse(string &s);

public:
    Parser();

    TNode *parseProgram(string &s);

    /**
     * Parses an expression string into an abstract syntax tree (AST).
     * Used by the query evaluator for expression-spec.
     * expr -> expr + term | expr - term | term
     *
     * @param s the expr string with optional leading/trailing whitespace
     * @return the parsed AST
     */
    TNode *parseExpr(string &s);

    // testing purposes

    TNode *parseProcedure(string &s);

    TNode *parseStmtLst(string &s);

    TNode *parseStmt(string &s);

    TNode *parseRead(string &s);

    TNode *parsePrint(string &s);

    TNode *parseCall(string &s);

    TNode *parseWhile(string &s);

    TNode *parseIf(string &s);

    TNode *parseAssign(string &s);

    TNode *parseCondExpr(string &s);

    TNode *parseTerm(string &s);

    TNode *parseName(string &s);

    TNode *parseConst(string &s);

    // debugging purposes
    void printTokens();
};
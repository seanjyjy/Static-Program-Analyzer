#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <cassert>

using namespace std;

#include <stdexcept>

#include "Token.h"
#include "Common/TNode.h"
#include "Parser.h"
#include "Tokenizer.h"

Parser::Parser(): cursor(0) {}

void Parser::advance() {
    cursor++;
    assert(cursor < tokens.size()); // last token is eof
    currToken = tokens[cursor];
}

int Parser::saveCursor() {
    return cursor;
}

void Parser::backtrack(int to) {
    assert(to >= 0 && to < tokens.size());
    cursor = to;
    currToken = tokens[cursor];
}

void Parser::expect(TokenType type) {
    if (currToken.getType() != type) {
        string expectedType = Token::typeToString(type);
        string gotType = Token::typeToString(currToken.getType());
        auto[startRow, startCol] = currToken.getStart();
        string msg = "expected " + expectedType + " at row " + to_string(startRow) +
                     ", col " + to_string(startCol) + ", got " + gotType;
        throw runtime_error(msg);
    }
}

void Parser::expect(TokenType type, string s) {
    if (currToken.getType() != type || currToken.getVal() != s) {
        string expectedType = Token::typeToString(type);
        string gotType = Token::typeToString(currToken.getType());
        auto[startRow, startCol] = currToken.getStart();
        string msg = "expected " + expectedType + " " + s +
                     " at row " + to_string(startRow) + ", col " + to_string(startCol) +
                     ", got " + gotType + currToken.getVal();
        throw runtime_error(msg);
    }
}

bool Parser::peekMatchType(TokenType type) {
    return currToken.getType() == type;
}

bool Parser::peekMatchTypeVal(TokenType type, string val) {
    return peekMatchType(type) && currToken.getVal() == val;
}

string Parser::genErrorMsgWithCurrToken() {
    auto[startRow, startCol] = currToken.getStart();
    auto[endRow, endCol] = currToken.getEnd();
    TokenType t = currToken.getType();
    return "unexpected token " + Token::typeToString(t) + " from row " + to_string(startRow) + " col " +
           to_string(startCol) + " to row " + to_string(endRow) + " col " + to_string(endCol);
}

string Parser::genSyntaxErrorMsg() {
    auto[startRow, startCol] = currToken.getStart();
    return "syntax error while parsing tokens starting from row " + to_string(startRow) + " col " + to_string(startCol);
}

Token *Parser::checkAndAdvance(TokenType type) {
    expect(type);
    Token *ret = currToken.copy();
    advance();
    return ret;
}

Token *Parser::checkAndAdvance(TokenType type, string val) {
    expect(type, move(val));
    Token *ret = currToken.copy();
    advance();
    return ret;
}

TNode *Parser::eatProgram() {
    return TNode::makeProgram(eatProcedure());
}

// procedure -> 'procedure' proc_name '{' stmtLst '}'
TNode *Parser::eatProcedure() {
    // eat procedure keyword
    checkAndAdvance(TokenType::name, "procedure");

    // eat procedure name
    Token *name = checkAndAdvance(TokenType::name);

    // eat {
    checkAndAdvance(TokenType::openingBrace);

    // eat statements
    TNode *stmtLst = eatStmtLst();

    // eat }
    checkAndAdvance(TokenType::closingBrace);

    return TNode::makeProcedure(name, stmtLst);
}

// stmtLst -> stmt+
TNode *Parser::eatStmtLst() {
    vector<TNode *> stmts;
    while (!peekMatchType(TokenType::closingBrace)) {
        stmts.push_back(eatStmt());
    }
    return TNode::makeStmtLst(stmts);
}

// stmt -> read | print | call | while | if | assign
TNode *Parser::eatStmt() {
    if (peekMatchTypeVal(TokenType::name, "read")) return eatStmtRead();
    if (peekMatchTypeVal(TokenType::name, "print")) return eatStmtPrint();
    if (peekMatchTypeVal(TokenType::name, "call")) return eatStmtCall();
    if (peekMatchTypeVal(TokenType::name, "while")) return eatStmtWhile();
    if (peekMatchTypeVal(TokenType::name, "if")) return eatStmtIf();
    if (peekMatchType(TokenType::name)) return eatStmtAssign();
    throw runtime_error(genErrorMsgWithCurrToken());
}

// read -> 'read' var_name ';'
TNode *Parser::eatStmtRead() {
    checkAndAdvance(TokenType::name, "read");
    Token *tokenBeforeAdv = checkAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeReadStmt(TNode::makeVarName(tokenBeforeAdv));
}

// print -> 'print' var_name ';'
TNode *Parser::eatStmtPrint() {
    checkAndAdvance(TokenType::name, "print");
    Token *tokenBeforeAdv = checkAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makePrintStmt(TNode::makeVarName(tokenBeforeAdv));
}

// call -> 'call' proc_name ';'
TNode *Parser::eatStmtCall() {
    checkAndAdvance(TokenType::name, "call");
    Token *tokenBeforeAdv = checkAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeCallStmt(TNode::makeProcName(tokenBeforeAdv));
}

// while -> 'while' '(' cond_expr ')' '{' stmtLst '}'
TNode *Parser::eatStmtWhile() {
    checkAndAdvance(TokenType::name, "while");
    checkAndAdvance(TokenType::openingBracket);
    TNode *condExpr = eatCondExpr();
    checkAndAdvance(TokenType::closingBracket);
    checkAndAdvance(TokenType::openingBrace);
    TNode *stmtLst = eatStmtLst();
    checkAndAdvance(TokenType::closingBrace);
    return TNode::makeWhileStmt(condExpr, stmtLst);
}

// if -> 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
TNode *Parser::eatStmtIf() {
    checkAndAdvance(TokenType::name, "if");
    checkAndAdvance(TokenType::openingBracket);
    TNode *condExpr = eatCondExpr();
    checkAndAdvance(TokenType::closingBracket);
    checkAndAdvance(TokenType::name, "then");
    checkAndAdvance(TokenType::openingBrace);
    TNode *ifStmtLst = eatStmtLst();
    checkAndAdvance(TokenType::closingBrace);
    checkAndAdvance(TokenType::name, "else");
    checkAndAdvance(TokenType::openingBrace);
    TNode *elseStmtLst = eatStmtLst();
    checkAndAdvance(TokenType::closingBrace);
    return TNode::makeIfStmt(condExpr, ifStmtLst, elseStmtLst);
}

// assign -> var_name '=' expr ';'
TNode *Parser::eatStmtAssign() {
    Token *tokenBeforeAdv = checkAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::assign);
    TNode *expr = eatExpr();
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeAssignStmt(TNode::makeVarName(tokenBeforeAdv), expr);
}

/*
 * cond_expr  -> rel_expr
 *          | '!' '(' cond_expr ')'
 *          | '(' cond_expr ')' '&&' '(' cond_expr ')'
 *          | '(' cond_expr ')' '||' '(' cond_expr ')'
 */
TNode *Parser::eatCondExpr() {
    int c = saveCursor();
    try {
        return eatRelExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        checkAndAdvance(TokenType::notOp);
        checkAndAdvance(TokenType::openingBracket);
        TNode *condExpr = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return TNode::makeNot(condExpr);
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        checkAndAdvance(TokenType::openingBracket);
        TNode *condExpr1 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::andOp);
        checkAndAdvance(TokenType::openingBracket);
        TNode *condExpr2 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return TNode::makeAnd(condExpr1, condExpr2);
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        checkAndAdvance(TokenType::openingBracket);
        TNode *condExpr1 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::orOp);
        checkAndAdvance(TokenType::openingBracket);
        TNode *condExpr2 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return TNode::makeOr(condExpr1, condExpr2);
    } catch (exception &e) {
        backtrack(c);
    }

    // incorrect syntax
    throw runtime_error(genSyntaxErrorMsg());
}

/*
 * rel_expr   -> rel_factor '>' rel_factor
 *              | rel_factor '>=' rel_factor
 *              | rel_factor '<' rel_factor
 *              | rel_factor '<=' rel_factor
 *              | rel_factor '==' rel_factor
 *              | rel_factor '!=' rel_facto
 */
TNode *Parser::eatRelExpr() {
    TNode *rf1 = eatRelFactor();
    if (peekMatchType(TokenType::gt)) {
        checkAndAdvance(TokenType::gt);
        TNode *rf2 = eatRelFactor();
        return TNode::makeGt(rf1, rf2);
    } else if (peekMatchType(TokenType::ge)) {
        checkAndAdvance(TokenType::ge);
        TNode *rf2 = eatRelFactor();
        return TNode::makeGe(rf1, rf2);
    } else if (peekMatchType(TokenType::lt)) {
        checkAndAdvance(TokenType::lt);
        TNode *rf2 = eatRelFactor();
        return TNode::makeLt(rf1, rf2);
    } else if (peekMatchType(TokenType::le)) {
        checkAndAdvance(TokenType::le);
        TNode *rf2 = eatRelFactor();
        return TNode::makeLe(rf1, rf2);
    } else if (peekMatchType(TokenType::eq)) {
        checkAndAdvance(TokenType::eq);
        TNode *rf2 = eatRelFactor();
        return TNode::makeEq(rf1, rf2);
    } else if (peekMatchType(TokenType::ne)) {
        checkAndAdvance(TokenType::ne);
        TNode *rf2 = eatRelFactor();
        return TNode::makeNe(rf1, rf2);
    }
    throw runtime_error(genSyntaxErrorMsg());
}

/*
 * rel_factor -> var_name | const_value | expr
 */
TNode *Parser::eatRelFactor() {
    if (peekMatchType(TokenType::name)) {
        Token *t = checkAndAdvance(TokenType::name);
        return TNode::makeVarName(t);
    } else if (peekMatchType(TokenType::integer)) {
        Token *t = checkAndAdvance(TokenType::integer);
        return TNode::makeConstVal(t);
    } else {
        return eatExpr();
    }
}

// expr -> term expr1 | term
TNode *Parser::eatExpr() {
    TNode *term = eatTerm();

    int c = saveCursor();
    try {
        TNode *expr1 = eatExpr1();
        expr1->setLeftChild(term);
        while (expr1->getParent() != nullptr) expr1 = expr1->getParent();
        return expr1;
    } catch (exception &e) {
        backtrack(c);
    }

    return term;
}

// expr1 -> '+' term expr1 | '-' term expr1 | '+' term | '-' term
TNode *Parser::eatExpr1() {
    if (peekMatchType(TokenType::plus)) {
        checkAndAdvance(TokenType::plus);
        TNode *term = eatTerm();

        int c = saveCursor();
        try {
            TNode *expr1 = eatExpr1();
            TNode *pl = TNode::makePlus(TNode::makeDummy(), term);
            expr1->setLeftChild(pl);
            pl->setParent(expr1);
            return pl;
        } catch (exception &e) {
            backtrack(c);
        }

        return TNode::makePlus(TNode::makeDummy(), term);
    } else if (peekMatchType(TokenType::minus)) {
        checkAndAdvance(TokenType::minus);
        TNode *term = eatTerm();

        int c = saveCursor();
        try {
            TNode *expr1 = eatExpr1();
            TNode *pl = TNode::makeMinus(TNode::makeDummy(), term);
            expr1->setLeftChild(pl);
            pl->setParent(expr1);
            return pl;
        } catch (exception &e) {
            backtrack(c);
        }

        return TNode::makeMinus(TNode::makeDummy(), term);
    }
    throw runtime_error(genSyntaxErrorMsg());
}

// term -> factor term1 | factor
TNode *Parser::eatTerm() {
    TNode *factor = eatFactor();

    int c = saveCursor();
    try {
        TNode *term1 = eatTerm1();
        term1->setLeftChild(factor);
        while (term1->getParent() != nullptr) term1 = term1->getParent();
        return term1;
    } catch (exception &e) {
        backtrack(c);
    }

    return factor;
}

// term1 -> '*' factor term1 | '/' factor term1 | '%' factor term1 | '*' factor | '/' factor | '%' factor
TNode *Parser::eatTerm1() {
    if (peekMatchType(TokenType::times)) {
        checkAndAdvance(TokenType::times);
        TNode *factor = eatFactor();

        int c = saveCursor();
        try {
            TNode *term1 = eatTerm1();
            TNode *times = TNode::makeTimes(TNode::makeDummy(), factor);
            term1->setLeftChild(times);
            times->setParent(term1);
            return times;
        } catch (exception &e) {
            backtrack(c);
        }

        return TNode::makeTimes(TNode::makeDummy(), factor);
    } else if (peekMatchType(TokenType::div)) {
        checkAndAdvance(TokenType::div);
        TNode *factor = eatFactor();

        int c = saveCursor();
        try {
            TNode *term1 = eatTerm1();
            TNode *dv = TNode::makeDiv(TNode::makeDummy(), factor);
            term1->setLeftChild(dv);
            dv->setParent(term1);
            return dv;
        } catch (exception &e) {
            backtrack(c);
        }

        return TNode::makeDiv(TNode::makeDummy(), factor);
    } else if (peekMatchType(TokenType::mod)) {
        checkAndAdvance(TokenType::mod);
        TNode *factor = eatFactor();

        int c = saveCursor();
        try {
            TNode *term1 = eatTerm1();
            TNode *md = TNode::makeMod(TNode::makeDummy(), factor);
            term1->setLeftChild(md);
            md->setParent(term1);
            return md;
        } catch (exception &e) {
            backtrack(c);
        }

        return TNode::makeMod(TNode::makeDummy(), factor);
    }
    throw runtime_error(genSyntaxErrorMsg());
}

// factor -> var_name | const_value | '(' expr ')'
TNode *Parser::eatFactor() {
    if (peekMatchType(TokenType::name)) {
        Token *t = checkAndAdvance(TokenType::name);
        return TNode::makeVarName(t);
    } else if (peekMatchType(TokenType::integer)) {
        Token *t = checkAndAdvance(TokenType::integer);
        return TNode::makeConstVal(t);
    } else {
        checkAndAdvance(TokenType::openingBracket);
        TNode *expr = eatExpr();
        checkAndAdvance(TokenType::closingBracket);
        return expr;
    }
}

TNode *Parser::parseProgram(string &s) {
    init(s);

    // parse to ast
    try {
        TNode *ast = eatProgram();
        if (!peekMatchType(TokenType::eof)) throw runtime_error("invalid program syntax");
        // success, now set all parent pointers
        ast->setAllParents();
        return ast;
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    return nullptr;
}

TNode *Parser::parseExpr(string &s) {
    init(s);

    try {
        TNode *ast = eatExpr();
        if (!peekMatchType(TokenType::eof)) throw runtime_error("invalid expr syntax");
        // success, now set all parent pointers
        ast->setAllParents();
        return ast;
    } catch (exception &e) {
        cout << e.what() << endl;
    }

    return nullptr;
}


void Parser::printTokens() {
    tokens.print();
}

void Parser::init(string &s) {
    // set current string to parse
    input = move(s);

    // remove previous tokens (if any)
    tokens.clear();

    // tokenize input
    Tokenizer tokenizer(input);
    tokens = tokenizer.tokenize();

    // tokenize succeeded, now setup for ast recursive descent
    if (tokens.empty()) throw runtime_error("parser tokens must not be empty");
    cursor = 0;
    currToken = tokens[0];
}



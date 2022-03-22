#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <stdexcept>

using namespace std;

#include "Token.h"
#include "Parser.h"
#include "Tokenizer.h"
#include "Exception/SyntaxException.h"
#include "Exception/TokenizeException.h"
#include "SimpleParser/SPUtils.h"

Parser::Parser() = default;

void Parser::advance() {
    cursor++;
    if (cursor >= (int) tokens.size()) throw runtime_error("cursor index out of bounds"); // last token is eof
    currToken = tokens[cursor];
}

int Parser::saveCursor() {
    return cursor;
}

void Parser::backtrack(int to) {
    if (to < 0 || to >= (int) tokens.size()) throw runtime_error("invalid backtrack index");
    cursor = to;
    currToken = tokens[cursor];
}

void Parser::expect(TokenType type) {
    if (currToken.getType() != type) {
        string expectedType = Token::typeToString(type);
        string gotType = Token::typeToString(currToken.getType());
        auto[startRow, startCol] = currToken.getStart();
        auto[endRow, endCol] = currToken.getEnd();
        if (startRow < errorStartRow || (startRow == errorStartRow && startCol < errorStartCol)) {
            errorStartRow = startRow;
            errorStartCol = startCol;
        }
        if (endRow > errorEndRow || (endRow == errorEndRow && endCol > errorEndCol)) {
            errorEndRow = endRow;
            errorEndCol = endCol;
        }
        throw SyntaxException();
    }
}

void Parser::expect(TokenType type, const string &s) {
    if (currToken.getType() != type || currToken.getVal() != s) {
        string expectedType = Token::typeToString(type);
        string gotType = Token::typeToString(currToken.getType());
        auto[startRow, startCol] = currToken.getStart();
        auto[endRow, endCol] = currToken.getEnd();
        if (startRow < errorStartRow || (startRow == errorStartRow && startCol < errorStartCol)) {
            errorStartRow = startRow;
            errorStartCol = startCol;
        }
        if (endRow > errorEndRow || (endRow == errorEndRow && endCol > errorEndCol)) {
            errorEndRow = endRow;
            errorEndCol = endCol;
        }
        throw SyntaxException();
    }
}

bool Parser::peekMatchType(TokenType type) {
    return currToken.getType() == type;
}

bool Parser::isEof() {
    return peekMatchType(TokenType::eof);
}

string Parser::withDetails(const string &s) {
    return s + " - from" +
           " row " + to_string(errorStartRow + 1) + " col " + to_string(errorStartCol + 1) +
           " to row " + to_string(errorEndRow + 1) + " col " + to_string(errorEndCol + 1);
}

string Parser::syntaxErrorMsg() {
    return withDetails("syntax error during parse");
}

string Parser::highlightSource() {
    return SPUtils::highlightAndBanner(
            input, errorStartRow, errorStartCol, errorEndRow, errorEndCol,
            "vvvvvvvvvvvvvvvvvvvvvvvvvvvvv PARSER ERROR HIGHLIGHT vvvvvvvvvvvvvvvvvvvvvvvvvvvvv",
            "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ END ERROR HIGHLIGHT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
    );
}

Token *Parser::checkAndGetTokenAndAdvance(TokenType type) {
    expect(type);
    Token *ret = currToken.copy();
    advance();
    return ret;
}

void Parser::checkAndAdvance(TokenType type) {
    expect(type);
    advance();
}

void Parser::checkAndAdvance(TokenType type, const string &val) {
    expect(type, val);
    advance();
}

Program *Parser::eatProgram() {
    vector<Procedure *> procedures;
    try {
        while (!isEof()) {
            procedures.push_back(eatProcedure());
        }
    } catch (SyntaxException &) {
        for (TNode *proc: procedures) {
            delete proc;
        }
        throw SyntaxException();
    }
    return new Program(procedures);
}

Procedure *Parser::eatProcedure() {
    Token *name = nullptr;
    StmtLst *stmtLst = nullptr;

    try {
        // eat procedure keyword
        checkAndAdvance(TokenType::name, "procedure");
        // eat procedure name
        name = checkAndGetTokenAndAdvance(TokenType::name);
        // eat {
        checkAndAdvance(TokenType::openingBrace);
        // eat statements
        stmtLst = eatStmtLst();
        // eat }
        checkAndAdvance(TokenType::closingBrace);

        return new Procedure(name, stmtLst);
    } catch (SyntaxException &) {
        delete name;
        delete stmtLst;
        throw;
    }
}

StmtLst *Parser::eatStmtLst() {
    vector<Stmt *> stmts;
    try {
        // at least one statement in stmtlist
        stmts.push_back(eatStmt());
        while (!peekMatchType(TokenType::closingBrace)) {
            stmts.push_back(eatStmt());
        }
        return new StmtLst(stmts);
    } catch (SyntaxException &) {
        for (TNode *stmt: stmts) {
            delete stmt;
        }
        throw;
    }
}

Stmt *Parser::eatStmt() {
    int c = saveCursor();
    try {
        return eatStmtRead();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtPrint();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtCall();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtWhile();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtIf();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtAssign();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    throw SyntaxException();
}

Read *Parser::eatStmtRead() {
    Token *tokenBeforeAdv = nullptr;
    try {
        checkAndAdvance(TokenType::name, "read");
        tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
        checkAndAdvance(TokenType::semicolon);
        return new Read(new VarName(tokenBeforeAdv));
    } catch (SyntaxException &) {
        delete tokenBeforeAdv;
        throw;
    }
}

Print *Parser::eatStmtPrint() {
    Token *tokenBeforeAdv = nullptr;
    try {
        checkAndAdvance(TokenType::name, "print");
        tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
        checkAndAdvance(TokenType::semicolon);
        return new Print(new VarName(tokenBeforeAdv));
    } catch (SyntaxException &) {
        delete tokenBeforeAdv;
        throw;
    }
}

Call *Parser::eatStmtCall() {
    Token *tokenBeforeAdv = nullptr;
    try {
        checkAndAdvance(TokenType::name, "call");
        tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
        checkAndAdvance(TokenType::semicolon);
        return new Call(new ProcName(tokenBeforeAdv));
    } catch (SyntaxException &) {
        delete tokenBeforeAdv;
        throw;
    }
}

While *Parser::eatStmtWhile() {
    TNode *condExpr = nullptr;
    StmtLst *stmtLst = nullptr;
    try {
        checkAndAdvance(TokenType::name, "while");
        checkAndAdvance(TokenType::openingBracket);
        condExpr = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::openingBrace);
        stmtLst = eatStmtLst();
        checkAndAdvance(TokenType::closingBrace);
        return new While(condExpr, stmtLst);
    } catch (SyntaxException &) {
        delete condExpr;
        delete stmtLst;
        throw;
    }
}

If *Parser::eatStmtIf() {
    TNode *condExpr = nullptr;
    StmtLst *ifStmtLst = nullptr;
    StmtLst *elseStmtLst = nullptr;
    try {
        checkAndAdvance(TokenType::name, "if");
        checkAndAdvance(TokenType::openingBracket);
        condExpr = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::name, "then");
        checkAndAdvance(TokenType::openingBrace);
        ifStmtLst = eatStmtLst();
        checkAndAdvance(TokenType::closingBrace);
        checkAndAdvance(TokenType::name, "else");
        checkAndAdvance(TokenType::openingBrace);
        elseStmtLst = eatStmtLst();
        checkAndAdvance(TokenType::closingBrace);
        return new If(condExpr, ifStmtLst, elseStmtLst);
    } catch (SyntaxException &) {
        delete condExpr;
        delete ifStmtLst;
        delete elseStmtLst;
        throw;
    }
}

Assign *Parser::eatStmtAssign() {
    Token *tokenBeforeAdv = nullptr;
    TNode *expr = nullptr;
    try {
        tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
        checkAndAdvance(TokenType::assign);
        expr = eatExpr();
        checkAndAdvance(TokenType::semicolon);
        return new Assign(new VarName(tokenBeforeAdv), expr);
    } catch (SyntaxException &) {
        delete tokenBeforeAdv;
        delete expr;
        throw;
    }
}

CondExpr *Parser::eatCondExpr() {
    int c = saveCursor();
    try {
        return eatRelExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    CondExpr *condExpr = nullptr;
    try {
        checkAndAdvance(TokenType::notOp);
        checkAndAdvance(TokenType::openingBracket);
        condExpr = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return new Not(condExpr);
    } catch (SyntaxException &) {
        delete condExpr;
        backtrack(c);
    }

    CondExpr *condExpr1 = nullptr;
    CondExpr *condExpr2 = nullptr;
    c = saveCursor();
    try {
        checkAndAdvance(TokenType::openingBracket);
        condExpr1 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::andOp);
        checkAndAdvance(TokenType::openingBracket);
        condExpr2 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return new And(condExpr1, condExpr2);
    } catch (SyntaxException &) {
        delete condExpr1;
        delete condExpr2;
        backtrack(c);
    }

    condExpr1 = nullptr;
    condExpr2 = nullptr;
    c = saveCursor();
    try {
        checkAndAdvance(TokenType::openingBracket);
        condExpr1 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::orOp);
        checkAndAdvance(TokenType::openingBracket);
        condExpr2 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return new Or(condExpr1, condExpr2);
    } catch (SyntaxException &) {
        delete condExpr1;
        delete condExpr2;
        backtrack(c);
    }

    // incorrect syntax
    throw SyntaxException();
}

RelExpr *Parser::eatRelExpr() {
    int c = saveCursor();

    try {
        return eatGtExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatGeExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatLtExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatLeExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatEqExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatNeExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    throw SyntaxException();
}

Gt *Parser::eatGtExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::gt);
    RelFactor *rf2 = eatRelFactor();
    return new Gt(rf1, rf2);
}

Ge *Parser::eatGeExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::ge);
    RelFactor *rf2 = eatRelFactor();
    return new Ge(rf1, rf2);
}

Lt *Parser::eatLtExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::lt);
    RelFactor *rf2 = eatRelFactor();
    return new Lt(rf1, rf2);
}

Le *Parser::eatLeExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::le);
    RelFactor *rf2 = eatRelFactor();
    return new Le(rf1, rf2);
}

Eq *Parser::eatEqExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::eq);
    RelFactor *rf2 = eatRelFactor();
    return new Eq(rf1, rf2);
}

Ne *Parser::eatNeExpr() {
    RelFactor *rf1 = eatRelFactor();
    checkAndAdvance(TokenType::ne);
    RelFactor *rf2 = eatRelFactor();
    return new Ne(rf1, rf2);
}

RelFactor *Parser::eatRelFactor() {
    int c = saveCursor();
    try {
        return eatExpr();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatVarName();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatConstVal();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    throw SyntaxException();
}

// expr -> term expr1 | term
RelFactor *Parser::eatExpr() {
    RelFactor *term = eatTerm();

    int c = saveCursor();
    RelFactor *expr1 = nullptr;
    try {
        expr1 = eatExpr1();
        expr1->setLeftChild(term);
        while (expr1->getRelParent() != nullptr) expr1 = expr1->getRelParent();
        return expr1;
    } catch (SyntaxException &) {
        delete expr1;
        backtrack(c);
    }

    return term;
}

RelFactor *Parser::eatExpr1() {
    if (peekMatchType(TokenType::plus)) {
        checkAndAdvance(TokenType::plus);
        RelFactor *term = eatTerm();

        int c = saveCursor();
        RelFactor *expr1 = nullptr;
        try {
            expr1 = eatExpr1();
            RelFactor *pl = new Plus(RelFactor::dummy(), term);
            expr1->setLeftChild(pl);
            pl->setRelParent(expr1);
            return pl;
        } catch (SyntaxException &) {
            delete expr1;
            backtrack(c);
        }

        return new Plus(RelFactor::dummy(), term);
    } else if (peekMatchType(TokenType::minus)) {
        checkAndAdvance(TokenType::minus);
        RelFactor *term = eatTerm();

        int c = saveCursor();
        RelFactor *expr1 = nullptr;
        try {
            expr1 = eatExpr1();
            RelFactor *pl = new Minus(RelFactor::dummy(), term);
            expr1->setLeftChild(pl);
            pl->setRelParent(expr1);
            return pl;
        } catch (SyntaxException &) {
            delete expr1;
            backtrack(c);
        }

        return new Minus(RelFactor::dummy(), term);
    }
    throw SyntaxException();
}

RelFactor *Parser::eatTerm() {
    RelFactor *factor = eatFactor();

    int c = saveCursor();
    RelFactor *term1 = nullptr;
    try {
        term1 = eatTerm1();
        term1->setLeftChild(factor);
        while (term1->getRelParent() != nullptr) term1 = term1->getRelParent();
        return term1;
    } catch (SyntaxException &) {
        delete term1;
        backtrack(c);
    }

    return factor;
}

RelFactor *Parser::eatTerm1() {
    if (peekMatchType(TokenType::times)) {
        checkAndAdvance(TokenType::times);
        RelFactor *factor = eatFactor();

        int c = saveCursor();
        RelFactor *term1 = nullptr;
        try {
            term1 = eatTerm1();
            RelFactor *times = new Times(RelFactor::dummy(), factor);
            term1->setLeftChild(times);
            times->setRelParent(term1);
            return times;
        } catch (SyntaxException &) {
            delete term1;
            backtrack(c);
        }

        return new Times(RelFactor::dummy(), factor);
    } else if (peekMatchType(TokenType::div)) {
        checkAndAdvance(TokenType::div);
        RelFactor *factor = eatFactor();

        int c = saveCursor();
        RelFactor *term1 = nullptr;
        RelFactor *dv = nullptr;
        try {
            term1 = eatTerm1();
            dv = new Div(RelFactor::dummy(), factor);
            term1->setLeftChild(dv);
            dv->setRelParent(term1);
            return dv;
        } catch (SyntaxException &) {
            delete term1;
            delete dv;
            backtrack(c);
        }

        return new Div(RelFactor::dummy(), factor);
    } else if (peekMatchType(TokenType::mod)) {
        checkAndAdvance(TokenType::mod);
        RelFactor *factor = eatFactor();

        int c = saveCursor();
        RelFactor *term1 = nullptr;
        try {
            term1 = eatTerm1();
            RelFactor *md = new Mod(RelFactor::dummy(), factor);
            term1->setLeftChild(md);
            md->setRelParent(term1);
            return md;
        } catch (SyntaxException &) {
            delete term1;
            backtrack(c);
        }

        return new Mod(RelFactor::dummy(), factor);
    }
    throw SyntaxException();
}

RelFactor *Parser::eatFactor() {
    int c = saveCursor();
    RelFactor *expr = nullptr;
    try {
        checkAndAdvance(TokenType::openingBracket);
        expr = eatExpr();
        checkAndAdvance(TokenType::closingBracket);
        return expr;
    } catch (SyntaxException &) {
        delete expr;
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatVarName();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatConstVal();
    } catch (SyntaxException &) {
        backtrack(c);
    }

    throw SyntaxException();
}

void Parser::init(const string &s) {
    // set current string to parse
    input = s;

    // remove previous tokens (if any)
    tokens.clear();

    // tokenize input
    Tokenizer tokenizer(input);
    tokens = tokenizer.tokenize();

    // tokenize succeeded, now setup for ast recursive descent
    if (tokens.empty()) throw runtime_error("parser tokens must not be empty");

    cursor = 0;
    errorStartRow = INT_MAX;
    errorStartCol = INT_MAX;
    errorEndRow = -1;
    errorEndCol = -1;
    currToken = tokens[0];
}

TNode *Parser::parse(const string &s) {
    try {
        // setup and tokenize
        init(s);

        TNode *ast = nullptr;
        switch (parseOption) {
            case Option::program:
                ast = eatProgram();
                break;
            case Option::procedure:
                ast = eatProcedure();
                break;
            case Option::stmt:
                ast = eatStmt();
                break;
            case Option::readStmt:
                ast = eatStmtRead();
                break;
            case Option::printStmt:
                ast = eatStmtPrint();
                break;
            case Option::callStmt:
                ast = eatStmtCall();
                break;
            case Option::whileStmt:
                ast = eatStmtWhile();
                break;
            case Option::ifStmt:
                ast = eatStmtIf();
                break;
            case Option::assignStmt:
                ast = eatStmtAssign();
                break;
            case Option::condExpr:
                ast = eatCondExpr();
                break;
            case Option::expr:
                ast = eatExpr();
                break;
            case Option::term:
                ast = eatTerm();
                break;
            case Option::name:
            case Option::constant:
                ast = eatFactor();
                break;
            default:
                throw runtime_error("unknown parse option given");
        }
        if (!peekMatchType(TokenType::eof)) throw runtime_error("extra tokens remaining after parse");
        // success, now set all parent pointers
        ast->setAllParents();
        return ast;
    } catch (TokenizeException &) {
        // tokenize errors are thrown pre-parse
        cout << "parser met an error during tokenizing, aborting" << endl;
    } catch (SyntaxException &) {
        // syntax errors are only thrown during parsing
        cout << syntaxErrorMsg() << endl;
        cout << endl << highlightSource() << endl;
    } catch (runtime_error &e) {
        // other exceptions not directly related to tokenizing/parsing
        cout << e.what() << endl;
    } catch (...) {
        // just in case
        cout << "unknown exception caught" << endl;
    }

    return nullptr;
}

TNode *Parser::parseProgram(const string &s) {
    parseOption = Option::program;
    return parse(s);
}

TNode *Parser::parseProcedure(const string &s) {
    parseOption = Option::procedure;
    return parse(s);
}

TNode *Parser::parseStmt(const string &s) {
    parseOption = Option::stmt;
    return parse(s);
}

TNode *Parser::parseRead(const string &s) {
    parseOption = Option::readStmt;
    return parse(s);
}

TNode *Parser::parsePrint(const string &s) {
    parseOption = Option::printStmt;
    return parse(s);
}

TNode *Parser::parseCall(const string &s) {
    parseOption = Option::callStmt;
    return parse(s);
}

TNode *Parser::parseWhile(const string &s) {
    parseOption = Option::whileStmt;
    return parse(s);
}

TNode *Parser::parseIf(const string &s) {
    parseOption = Option::ifStmt;
    return parse(s);
}

TNode *Parser::parseAssign(const string &s) {
    parseOption = Option::assignStmt;
    return parse(s);
}

TNode *Parser::parseCondExpr(const string &s) {
    parseOption = Option::condExpr;
    return parse(s);
}

TNode *Parser::parseExpr(const string &s) {
    parseOption = Option::expr;
    return parse(s);
}

TNode *Parser::parseTerm(const string &s) {
    parseOption = Option::term;
    return parse(s);
}

TNode *Parser::parseName(const string &s) {
    parseOption = Option::name;
    return parse(s);
}

TNode *Parser::parseConst(const string &s) {
    parseOption = Option::constant;
    return parse(s);
}

void Parser::printTokens() {
    tokens.print();
}

VarName *Parser::eatVarName() {
    Token *t = checkAndGetTokenAndAdvance(TokenType::name);
    return new VarName(t);
}

ConstVal *Parser::eatConstVal() {
    Token *t = checkAndGetTokenAndAdvance(TokenType::integer);
    return new ConstVal(t);
}

// remove comment to disable assertions before release
//#define NDEBUG
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
#include "Exception/SyntaxException.h"
#include "Exception/TokenizeException.h"
#include "Exception/ParseException.h"
#include "SimpleParser/SPUtils.h"

Parser::Parser() = default;

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
        auto [startRow, startCol] = currToken.getStart();
        auto [endRow, endCol] = currToken.getEnd();
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

void Parser::expect(TokenType type, const string& s) {
    if (currToken.getType() != type || currToken.getVal() != s) {
        string expectedType = Token::typeToString(type);
        string gotType = Token::typeToString(currToken.getType());
        auto [startRow, startCol] = currToken.getStart();
        auto [endRow, endCol] = currToken.getEnd();
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
           " row " + to_string(errorStartRow+1) + " col " + to_string(errorStartCol+1) +
           " to row " + to_string(errorEndRow+1) + " col " + to_string(errorEndCol+1);
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

void Parser::checkAndAdvance(TokenType type, const string& val) {
    expect(type, val);
    advance();
}

TNode *Parser::eatProgram() {
    vector<TNode *> procedures;
    while (!isEof()) {
        procedures.push_back(eatProcedure());
    }
    return TNode::makeProgram(procedures);
}

TNode *Parser::eatProcedure() {
    // eat procedure keyword
    checkAndAdvance(TokenType::name, "procedure");

    // eat procedure name
    Token *name = checkAndGetTokenAndAdvance(TokenType::name);

    // eat {
    checkAndAdvance(TokenType::openingBrace);

    // eat statements
    TNode *stmtLst = eatStmtLst();

    // eat }
    checkAndAdvance(TokenType::closingBrace);

    return TNode::makeProcedure(name, stmtLst);
}

TNode *Parser::eatStmtLst() {
    vector<TNode *> stmts;
    // at least one statement in stmtlist
    stmts.push_back(eatStmt());
    while (!peekMatchType(TokenType::closingBrace)) {
        stmts.push_back(eatStmt());
    }
    return TNode::makeStmtLst(stmts);
}

TNode *Parser::eatStmt() {
    int c = saveCursor();
    try {
        return eatStmtRead();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtPrint();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtCall();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtWhile();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtIf();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatStmtAssign();
    } catch (exception &e) {
        backtrack(c);
    }

    throw SyntaxException();
}

TNode *Parser::eatStmtRead() {
    checkAndAdvance(TokenType::name, "read");
    Token *tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeReadStmt(TNode::makeVarName(tokenBeforeAdv));
}

TNode *Parser::eatStmtPrint() {
    checkAndAdvance(TokenType::name, "print");
    Token *tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makePrintStmt(TNode::makeVarName(tokenBeforeAdv));
}

TNode *Parser::eatStmtCall() {
    checkAndAdvance(TokenType::name, "call");
    Token *tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeCallStmt(TNode::makeProcName(tokenBeforeAdv));
}

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

TNode *Parser::eatStmtAssign() {
    Token *tokenBeforeAdv = checkAndGetTokenAndAdvance(TokenType::name);
    checkAndAdvance(TokenType::assign);
    TNode *expr = eatExpr();
    checkAndAdvance(TokenType::semicolon);
    return TNode::makeAssignStmt(TNode::makeVarName(tokenBeforeAdv), expr);
}

TNode *Parser::eatCondExpr() {
    int c = saveCursor();
    try {
        return eatRelExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    TNode *condExpr = nullptr;
    try {
        checkAndAdvance(TokenType::notOp);
        checkAndAdvance(TokenType::openingBracket);
        condExpr = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return TNode::makeNot(condExpr);
    } catch (exception &e) {
        delete condExpr;
        backtrack(c);
    }

    TNode *condExpr1 = nullptr;
    TNode *condExpr2 = nullptr;
    c = saveCursor();
    try {
        checkAndAdvance(TokenType::openingBracket);
        condExpr1 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        checkAndAdvance(TokenType::andOp);
        checkAndAdvance(TokenType::openingBracket);
        condExpr2 = eatCondExpr();
        checkAndAdvance(TokenType::closingBracket);
        return TNode::makeAnd(condExpr1, condExpr2);
    } catch (exception &e) {
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
        return TNode::makeOr(condExpr1, condExpr2);
    } catch (exception &e) {
        delete condExpr1;
        delete condExpr2;
        backtrack(c);
    }

    // incorrect syntax
    throw SyntaxException();
}

TNode *Parser::eatRelExpr() {
    int c = saveCursor();

    try {
        return eatGtExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatGeExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatLtExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatLeExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatEqExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatNeExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    throw SyntaxException();
}

TNode *Parser::eatRelExpr(TokenType type) {
    TNode *rf1 = nullptr;
    TNode *rf2 = nullptr;
    // this try-catch is needed to deallocate memory if parsing fails
    try {
        switch (type) {
            case TokenType::gt: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::gt);
                rf2 = eatRelFactor();
                return TNode::makeGt(rf1, rf2);
            }
            case TokenType::ge: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::ge);
                rf2 = eatRelFactor();
                return TNode::makeGe(rf1, rf2);
            }
            case TokenType::lt: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::lt);
                rf2 = eatRelFactor();
                return TNode::makeLt(rf1, rf2);
            }
            case TokenType::le: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::le);
                rf2 = eatRelFactor();
                return TNode::makeLe(rf1, rf2);
            }
            case TokenType::eq: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::eq);
                rf2 = eatRelFactor();
                return TNode::makeEq(rf1, rf2);
            }
            case TokenType::ne: {
                rf1 = eatRelFactor();
                checkAndAdvance(TokenType::ne);
                rf2 = eatRelFactor();
                return TNode::makeNe(rf1, rf2);
            }
            default: {
                throw ParseException("incorrect token type given to eatRelExpr");
            }
        }
    } catch (exception &e) {
        delete rf1;
        delete rf2;
        throw;
    }
}

TNode *Parser::eatGtExpr() {
    return eatRelExpr(TokenType::gt);
}

TNode *Parser::eatGeExpr() {
    return eatRelExpr(TokenType::ge);
}

TNode *Parser::eatLtExpr() {
    return eatRelExpr(TokenType::lt);
}

TNode *Parser::eatLeExpr() {
    return eatRelExpr(TokenType::le);
}

TNode *Parser::eatEqExpr() {
    return eatRelExpr(TokenType::eq);
}

TNode *Parser::eatNeExpr() {
    return eatRelExpr(TokenType::ne);
}

TNode *Parser::eatRelFactor() {
    int c = saveCursor();
    try {
        return eatExpr();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatVarName();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatConstVal();
    } catch (exception &e) {
        backtrack(c);
    }

    throw SyntaxException();
}

// expr -> term expr1 | term
TNode *Parser::eatExpr() {
    TNode *term = eatTerm();

    int c = saveCursor();
    TNode *expr1 = nullptr;
    try {
        expr1 = eatExpr1();
        expr1->setLeftChild(term);
        while (expr1->getParent() != nullptr) expr1 = expr1->getParent();
        return expr1;
    } catch (exception &e) {
        delete expr1;
        backtrack(c);
    }

    return term;
}

TNode *Parser::eatExpr1() {
    if (peekMatchType(TokenType::plus)) {
        checkAndAdvance(TokenType::plus);
        TNode *term = eatTerm();

        int c = saveCursor();
        TNode *expr1 = nullptr;
        try {
            expr1 = eatExpr1();
            TNode *pl = TNode::makePlus(TNode::makeDummy(), term);
            expr1->setLeftChild(pl);
            pl->setParent(expr1);
            return pl;
        } catch (exception &e) {
            delete expr1;
            backtrack(c);
        }

        return TNode::makePlus(TNode::makeDummy(), term);
    } else if (peekMatchType(TokenType::minus)) {
        checkAndAdvance(TokenType::minus);
        TNode *term = eatTerm();

        int c = saveCursor();
        TNode *expr1 = nullptr;
        try {
            expr1 = eatExpr1();
            TNode *pl = TNode::makeMinus(TNode::makeDummy(), term);
            expr1->setLeftChild(pl);
            pl->setParent(expr1);
            return pl;
        } catch (exception &e) {
            delete expr1;
            backtrack(c);
        }

        return TNode::makeMinus(TNode::makeDummy(), term);
    }
    throw SyntaxException();
}

TNode *Parser::eatTerm() {
    TNode *factor = eatFactor();

    int c = saveCursor();
    TNode *term1 = nullptr;
    try {
        term1 = eatTerm1();
        term1->setLeftChild(factor);
        while (term1->getParent() != nullptr) term1 = term1->getParent();
        return term1;
    } catch (exception &e) {
        delete term1;
        backtrack(c);
    }

    return factor;
}

TNode *Parser::eatTerm1() {
    if (peekMatchType(TokenType::times)) {
        checkAndAdvance(TokenType::times);
        TNode *factor = eatFactor();

        int c = saveCursor();
        TNode *term1 = nullptr;
        try {
            term1 = eatTerm1();
            TNode *times = TNode::makeTimes(TNode::makeDummy(), factor);
            term1->setLeftChild(times);
            times->setParent(term1);
            return times;
        } catch (exception &e) {
            delete term1;
            backtrack(c);
        }

        return TNode::makeTimes(TNode::makeDummy(), factor);
    } else if (peekMatchType(TokenType::div)) {
        checkAndAdvance(TokenType::div);
        TNode *factor = eatFactor();

        int c = saveCursor();
        TNode *term1 = nullptr;
        try {
            TNode *term1 = eatTerm1();
            TNode *dv = TNode::makeDiv(TNode::makeDummy(), factor);
            term1->setLeftChild(dv);
            dv->setParent(term1);
            return dv;
        } catch (exception &e) {
            delete term1;
            backtrack(c);
        }

        return TNode::makeDiv(TNode::makeDummy(), factor);
    } else if (peekMatchType(TokenType::mod)) {
        checkAndAdvance(TokenType::mod);
        TNode *factor = eatFactor();

        int c = saveCursor();
        TNode *term1 = nullptr;
        try {
            term1 = eatTerm1();
            TNode *md = TNode::makeMod(TNode::makeDummy(), factor);
            term1->setLeftChild(md);
            md->setParent(term1);
            return md;
        } catch (exception &e) {
            delete term1;
            backtrack(c);
        }

        return TNode::makeMod(TNode::makeDummy(), factor);
    }
    throw SyntaxException();
}

TNode *Parser::eatFactor() {
    int c = saveCursor();
    TNode *expr = nullptr;
    try {
        checkAndAdvance(TokenType::openingBracket);
        expr = eatExpr();
        checkAndAdvance(TokenType::closingBracket);
        return expr;
    } catch (exception &e) {
        delete expr;
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatVarName();
    } catch (exception &e) {
        backtrack(c);
    }

    c = saveCursor();
    try {
        return eatConstVal();
    } catch (exception &e) {
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
    if (tokens.empty()) throw exception("parser tokens must not be empty");

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
                throw exception("unknown parse option given");
        }
        if (!peekMatchType(TokenType::eof)) throw exception("extra tokens remaining after parse");
        // success, now set all parent pointers
        ast->setAllParents();
        return ast;
    } catch (TokenizeException &e) {
        // tokenize errors are thrown pre-parse
        cout << "parser met an error during tokenizing, aborting" << endl;
    } catch (ParseException &e) {
        // parse exceptions means parser business logic is wrong
        cout << "parser met an error during parsing, aborting" << endl;
    } catch (SyntaxException &e) {
        // syntax errors are only thrown during parsing
        cout << syntaxErrorMsg() << endl;
        cout << endl << highlightSource() << endl;
    } catch (exception &e) {
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

TNode *Parser::eatVarName() {
    Token *t = checkAndGetTokenAndAdvance(TokenType::name);
    return TNode::makeVarName(t);
}

TNode *Parser::eatConstVal() {
    Token *t = checkAndGetTokenAndAdvance(TokenType::integer);
    return TNode::makeConstVal(t);
}

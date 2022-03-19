#pragma once

#include "TNode.h"
#include "Common/AstNode/Program.h"
#include "Common/AstNode/Procedure.h"
#include "Common/AstNode/StmtLst.h"
#include "Common/AstNode/Read.h"
#include "Common/AstNode/Print.h"
#include "Common/AstNode/Call.h"
#include "Common/AstNode/While.h"
#include "Common/AstNode/If.h"
#include "Common/AstNode/Assign.h"
#include "Common/AstNode/UnaryOp.h"
#include "Common/AstNode/Gt.h"
#include "Common/AstNode/Ge.h"
#include "Common/AstNode/Lt.h"
#include "Common/AstNode/Le.h"
#include "Common/AstNode/Ne.h"
#include "Common/AstNode/Eq.h"
#include "Common/AstNode/VarName.h"
#include "Common/AstNode/ConstVal.h"
#include "Common/AstNode/ProcName.h"
#include "Common/AstNode/Dummy.h"
#include "Common/AstNode/Plus.h"
#include "Common/AstNode/Minus.h"
#include "Common/AstNode/Times.h"
#include "Common/AstNode/Div.h"
#include "Common/AstNode/Mod.h"
#include <stack>
#include <unordered_map>

using namespace std;

// Represents a parsed xml tag.
class XmlTag {
private:
    // accepted names in xml tag, e.g <program> ... </program>
    const string T_PROGRAM = "program";
    const string T_PROCEDURE = "procedure";
    const string T_STMTLIST = "stmts";
    const string T_READ = "read";
    const string T_PRINT = "print";
    const string T_CALL = "call";
    const string T_WHILE = "while";
    const string T_IF = "if";
    const string T_ASSIGN = "assign";
    const string T_NOT = "not";
    const string T_AND = "and";
    const string T_OR = "or";
    const string T_GT = "gt";
    const string T_GE = "ge";
    const string T_LT = "lt";
    const string T_LE = "le";
    const string T_EQ = "eq";
    const string T_NE = "ne";
    const string T_VAR = "var";
    const string T_PROC = "proc";
    const string T_CONST = "const";
    const string T_PLUS = "plus";
    const string T_MINUS = "minus";
    const string T_TIMES = "times";
    const string T_DIV = "div";
    const string T_MOD = "mod";

    // returns an empty program ast node.
    TNode *convertProgram();

    // reads procedure name "name" from data and returns a partially populated procedure ast node.
    TNode *convertProcedure();

    // returns an empty statement list ast node.
    TNode *convertStmtList();

    // returns an empty read node.
    TNode *convertRead();

    // returns an empty print ast node.
    TNode *convertPrint();

    // returns an empty call ast node.
    TNode *convertCall();

    // returns an empty while ast node.
    TNode *convertWhile();

    // returns an empty if ast node.
    TNode *convertIf();

    // returns an empty assign ast node.
    TNode *convertAssign();

    // returns an empty not ast node.
    TNode *convertNot();

    // returns an empty and ast node.
    TNode *convertAnd();

    // returns an empty or ast node.
    TNode *convertOr();

    // returns an empty > ast node.
    TNode *convertGt();

    // returns an empty >= ast node.
    TNode *convertGe();

    // returns an empty < ast node.
    TNode *convertLt();

    // returns an empty <= ast node.
    TNode *convertLe();

    // returns an empty == ast node.
    TNode *convertEq();

    // returns an empty != ast node.
    TNode *convertNe();

    // reads variable name "name" from data and returns a variable ast node.
    TNode *convertVar();

    // reads process name "name" from data and returns a process name ast node.
    TNode *convertProcName();

    // reads constant value "val" from data and returns an integer ast node.
    TNode *convertConst();

    // returns an empty + ast node.
    TNode *convertPlus();

    // returns an empty - ast node.
    TNode *convertMinus();

    // returns an empty * ast node.
    TNode *convertTimes();

    // returns an empty / ast node.
    TNode *convertDiv();

    // returns an empty % ast node.
    TNode *convertMod();

    // checks that all given keys are present in the data map, throws otherwise.
    void ensureKeys(const vector<string> &keys);

public:
    string type; // same as types given above
    unordered_map<string, string> data; // e.g <var name=hi></var>, key: name, value: "hi"
    bool isCloseTag; // e.g </var>

    XmlTag();

    XmlTag(string type, unordered_map<string, string> data, bool isCloseTag);

    // converts the xml tag to its corresponding AST node.
    TNode *convert();
};

class AstBuilder {
private:
    const char EOF_TOKEN = '\0';
    const char OPEN_TAG = '<';
    const char CLOSE_TAG = '>';

    string xml; // the input string in "xml" format
    int idx; // index position in the input string
    char currToken; // current char mirroring index position
    stack<pair<TNode *, bool>> stk; // { node, isOpeningTag }
    stack<XmlTag> tagStk; // mirrors stk, meant to ensure correctness

    // moves idx forward by one and updates the current char.
    void advance();

    // eats '<', an optional '/', a name (alphanumeric), optional key=value pairs, then '>', in that order.
    XmlTag eatXmlTag();

    // consumes the current char until a non-whitespace (by isspace(c)) is met.
    void eatWhitespace();

    // consumes '<'.
    void eatOpenTag();

    // consumes '>'.
    void eatCloseTag();

    // consumes '='.
    void eatAssign();

    // returns true if '/' is the current token, false otherwise. If true, '/' is consumed.
    bool eatSlashIfExists();

    // consumes an alphanumeric contiguous string.
    string eatName();

    // consumes multiple patterns of <ws>key<ws>=<ws>val<ws>, until a closing tag is met. <ws> = whitespace
    unordered_map<string, string> eatData();

    // returns true if the current token is EOF_TOKEN, false otherwise.
    bool isEof();

    // returns true if the current token is '>', false otherwise.
    bool isCloseTag();

    // returns true if the current token is '<', false otherwise.
    bool isOpenTag();

public:

    AstBuilder();

    AstBuilder(string xml);

    // returns an ast node based on the input xml; only returns a correct representation on first call.
    TNode *build();
};

#pragma once

#include "TNode.h"
#include <stack>
#include <unordered_map>

using namespace std;

class XmlTag {
private:
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
    const string T_CONST = "const";
    const string T_PLUS = "plus";
    const string T_MINUS = "minus";
    const string T_TIMES = "times";
    const string T_DIV = "div";
    const string T_MOD = "mod";

    TNode* convertProgram();
    TNode* convertProcedure();
    TNode* convertStmtList();
    TNode* convertRead();
    TNode* convertPrint();
    TNode* convertCall();
    TNode* convertWhile();
    TNode* convertIf();
    TNode* convertAssign();
    TNode* convertNot();
    TNode* convertOr();
    TNode* convertGt();
    TNode* convertGe();
    TNode* convertLt();
    TNode* convertLe();
    TNode* convertEq();
    TNode* convertNe();
    TNode* convertVar();
    TNode* convertConst();
    TNode* convertPlus();
    TNode* convertMinus();
    TNode* convertTimes();
    TNode* convertDiv();
    TNode* convertMod();

    void ensureKeys(const vector<string>& keys);
public:
    string type;
    unordered_map<string, string> data;
    bool isCloseTag;

    XmlTag();
    XmlTag(string type, unordered_map<string, string> data, bool isCloseTag);

    TNode* convert();
};

class AstBuilder {
private:
    const char EOF_TOKEN = '\0';
    const char OPEN_TAG = '<';
    const char CLOSE_TAG = '>';

    string xml;
    int idx;
    char currToken;
    stack<pair<TNode*, bool>> stk; // node/token, isOpeningTag
    stack<XmlTag> tagStk; // mirrors stk, meant to ensure correctness

    void advance();
    XmlTag eatXmlTag();
    void eatWhitespace();
    void eatOpenTag();
    void eatCloseTag();
    void eatAssign();
    bool eatSlashIfExists();
    string eatName();
    unordered_map<string, string> eatData();

    bool isEof();
    bool isCloseTag();
    bool isOpenTag();
public:

    AstBuilder();
    AstBuilder(string xml);

    TNode* build();
};

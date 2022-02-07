#include "AstBuilder.h"

#include <utility>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <iostream>

using namespace std;

AstBuilder::AstBuilder() = default;

AstBuilder::AstBuilder(string xml): xml(xml), idx(0) {
    if (xml.empty()) throw runtime_error("xml must be nonempty");
    currToken = xml[0];
}

TNode *AstBuilder::build() {
    while (!isEof()) {
        eatWhitespace();

        // parse xml
        XmlTag tag = eatXmlTag();

        if (!tag.isCloseTag) {
            // start of an xml tag, e.g <assign>
            stk.push({tag.convert(), true});
        } else {
            // end of an xml tag, e.g </assign>
            vector<TNode*> children;
            while (!stk.empty() && !stk.top().second) {
                auto [node, isProcessed] = stk.top();
                stk.pop();
                children.push_back(node);
            }

            // xml opening tag corresponding to closing tag
            auto [parent, isProcessed] = stk.top();
            stk.pop();

            // could be node or token
            if (parent->getType() == TNodeType::varName || parent->getType() == TNodeType::constValue) {
                stk.push({parent, false});
            } else {
                // left to right
                reverse(children.begin(), children.end());
                parent->setChildren(children);
                stk.push({parent, false});
            }
        }

        eatWhitespace();
    }

    assert(stk.size() == 1);
    auto [node, isProcessed] = stk.top();
    assert(node != nullptr);
    return node;
}

void AstBuilder::advance() {
    idx++;
    currToken = (idx >= xml.size()) ? EOF_TOKEN : xml[idx];
}

XmlTag AstBuilder::eatXmlTag() {
    // <SelfClosingTag/> or <OpeningTag> or </ClosingTag>

    // first, eat opening tag
    eatOpenTag();
    eatWhitespace();

    // possible closing tag
    bool isCloseTag = eatSlashIfExists();
    eatWhitespace();

    // eat type of tag
    string nType = eatName();
    eatWhitespace();

    // eat data of tag <Tag a=b c=d>
    unordered_map<string, string> data = eatData();
    eatWhitespace();

    // closing tag
    eatCloseTag();

    return {nType, data, isCloseTag};
}

void AstBuilder::eatWhitespace() {
    while (isspace(currToken)) {
        advance();
    }
}

bool AstBuilder::isCloseTag() {
    return currToken == CLOSE_TAG;
}

bool AstBuilder::isOpenTag() {
    return currToken == OPEN_TAG;
}

void AstBuilder::eatOpenTag() {
    assert(currToken == OPEN_TAG);
    advance();
}

void AstBuilder::eatCloseTag() {
    assert(isCloseTag());
    advance();
}

bool AstBuilder::eatSlashIfExists() {
    if (currToken == '/') {
        advance();
        return true;
    }
    return false;
}

string AstBuilder::eatName() {
    assert(!isspace(currToken));
    string ret;
    while (isalnum(currToken)) {
        ret += currToken;
        advance();
    }
    return ret;
}

bool AstBuilder::isEof() {
    return currToken == EOF_TOKEN;
}

void AstBuilder::eatAssign() {
    assert(currToken == '=');
    advance();
}

unordered_map<string, string> AstBuilder::eatData() {
    unordered_map<string, string> ret;
    while (!isCloseTag()) {
        eatWhitespace();
        string key = eatName();
        eatWhitespace();
        eatAssign();
        eatWhitespace();
        string value = eatName();
        eatWhitespace();
        ret[key] = value;
    }
    return ret;
}

/**
 * ********************* XML TAG ***********************
 */
XmlTag::XmlTag() = default;

XmlTag::XmlTag(string type, unordered_map<string, string> data, bool isCloseTag): type(move(type)), data(move(data)), isCloseTag(isCloseTag) {}

TNode* XmlTag::convert() {
    if (type == T_PROGRAM) return convertProgram();
    if (type == T_PROCEDURE) return convertProcedure();
    if (type == T_STMTLIST) return convertStmtList();
    if (type == T_READ) return convertRead();
    if (type == T_PRINT) return convertPrint();
    if (type == T_CALL) return convertCall();
    if (type == T_WHILE) return convertWhile();
    if (type == T_IF) return convertIf();
    if (type == T_ASSIGN) return convertAssign();
    if (type == T_NOT) return convertNot();
    if (type == T_OR) return convertOr();
    if (type == T_GT) return convertGt();
    if (type == T_GE) return convertGe();
    if (type == T_LT) return convertLt();
    if (type == T_LE) return convertLe();
    if (type == T_EQ) return convertEq();
    if (type == T_NE) return convertNe();
    if (type == T_VAR) return convertVar();
    if (type == T_CONST) return convertConst();
    throw runtime_error("unknown xml tag " + type);
}

TNode* XmlTag::convertProgram() {
    return TNode::makeProgram(nullptr);
}

TNode* XmlTag::convertProcedure() {
    string name = data["val"];
    return TNode::makeProcedure(new Token(TokenType::name, name), nullptr);
}

TNode* XmlTag::convertStmtList() {
    return TNode::makeStmtLst({});
}

TNode* XmlTag::convertRead() {
    return TNode::makeReadStmt(nullptr);
}

TNode* XmlTag::convertPrint() {
    return TNode::makePrintStmt(nullptr);
}

TNode* XmlTag::convertCall() {
    return TNode::makeCallStmt(nullptr);
}

TNode* XmlTag::convertWhile() {
    return TNode::makeWhileStmt(nullptr, nullptr);
}

TNode* XmlTag::convertIf() {
    return TNode::makeIfStmt(nullptr, nullptr, nullptr);
}

TNode* XmlTag::convertAssign() {
    return TNode::makeAssignStmt(nullptr, nullptr);
}

TNode* XmlTag::convertNot() {
    return TNode::makeNot(nullptr);
}

TNode* XmlTag::convertOr() {
    return TNode::makeOr(nullptr, nullptr);
}

TNode* XmlTag::convertGt() {
    return TNode::makeGt(nullptr, nullptr);
}

TNode* XmlTag::convertGe() {
    return TNode::makeGe(nullptr, nullptr);
}

TNode* XmlTag::convertLt() {
    return TNode::makeLt(nullptr, nullptr);
}

TNode* XmlTag::convertLe() {
    return TNode::makeLe(nullptr, nullptr);
}

TNode* XmlTag::convertEq() {
    return TNode::makeEq(nullptr, nullptr);
}

TNode* XmlTag::convertNe() {
    // will be filled later
    return TNode::makeNe(nullptr, nullptr);
}

TNode* XmlTag::convertVar() {
    string name = data["val"];
    return TNode::makeVarName(new Token(TokenType::name, name));
}

TNode* XmlTag::convertConst() {
    string name = data["val"];
    return TNode::makeConstVal(new Token(TokenType::name, name));
}


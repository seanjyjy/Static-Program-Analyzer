#include "AstBuilder.h"

#include <utility>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

AstBuilder::AstBuilder() = default;

AstBuilder::AstBuilder(string xml) : xml(xml), idx(0) {
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
            tagStk.push(tag);
        } else {
            // end of an xml tag, e.g </assign>
            vector<TNode *> children;
            while (!stk.empty() && !tagStk.empty() && !stk.top().second) {
                auto[node, isProcessed] = stk.top();
                stk.pop();
                tagStk.pop();
                children.push_back(node);
            }

            // xml opening tag corresponding to closing tag
            auto[parent, isProcessed] = stk.top();
            stk.pop();
            XmlTag parentTag = tagStk.top();
            tagStk.pop();

            // opening and closing tags must match
            if (tag.type != parentTag.type)
                throw runtime_error("xml tag mismatch: " + tag.type + ", " + parentTag.type);

            // could be node or token
            if (parent->getType() == TNodeType::varName || parent->getType() == TNodeType::constValue) {
                stk.push({parent, false});
                tagStk.push(parentTag);
            } else {
                // left to right
                reverse(children.begin(), children.end());
                parent->setChildren(children);
                stk.push({parent, false});
                tagStk.push(parentTag);
            }
        }

        eatWhitespace();
    }

    if (stk.size() != 1) throw runtime_error("stack size should be 1");
    auto[node, isProcessed] = stk.top();
    if (node == nullptr) throw runtime_error("node should not be null");
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
    if (!isOpenTag()) throw runtime_error("current token should be open tag");
    advance();
}

void AstBuilder::eatCloseTag() {
    if (!isCloseTag()) throw runtime_error("current token should be close tag)");
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
    if (isspace(currToken)) throw runtime_error("current token should not be <space>");
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
    if (currToken != '=') throw runtime_error("current token should be '='");
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

XmlTag::XmlTag(string type, unordered_map<string, string> data, bool isCloseTag) : type(move(type)), data(move(data)),
                                                                                   isCloseTag(isCloseTag) {}

TNode *XmlTag::convert() {
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
    if (type == T_AND) return convertAnd();
    if (type == T_OR) return convertOr();
    if (type == T_GT) return convertGt();
    if (type == T_GE) return convertGe();
    if (type == T_LT) return convertLt();
    if (type == T_LE) return convertLe();
    if (type == T_EQ) return convertEq();
    if (type == T_NE) return convertNe();
    if (type == T_VAR) return convertVar();
    if (type == T_PROC) return convertProcName();
    if (type == T_CONST) return convertConst();
    if (type == T_PLUS) return convertPlus();
    if (type == T_MINUS) return convertMinus();
    if (type == T_TIMES) return convertTimes();
    if (type == T_DIV) return convertDiv();
    if (type == T_MOD) return convertMod();
    throw runtime_error("unknown xml tag " + type);
}

TNode *XmlTag::convertProgram() {
    return TNode::makeProgram({});
}

TNode *XmlTag::convertProcedure() {
    ensureKeys({"name"});
    string name = data["name"];
    return TNode::makeProcedure(Token::makeName(name), nullptr);
}

TNode *XmlTag::convertStmtList() {
    return TNode::makeStmtLst({});
}

TNode *XmlTag::convertRead() {
    return TNode::makeReadStmt(nullptr);
}

TNode *XmlTag::convertPrint() {
    return TNode::makePrintStmt(nullptr);
}

TNode *XmlTag::convertCall() {
    return TNode::makeCallStmt(nullptr);
}

TNode *XmlTag::convertWhile() {
    return TNode::makeWhileStmt(nullptr, nullptr);
}

TNode *XmlTag::convertIf() {
    return TNode::makeIfStmt(nullptr, nullptr, nullptr);
}

TNode *XmlTag::convertAssign() {
    return TNode::makeAssignStmt(nullptr, nullptr);
}

TNode *XmlTag::convertNot() {
    return TNode::makeNot(nullptr);
}

TNode *XmlTag::convertAnd() {
    return TNode::makeAnd(nullptr, nullptr);
}

TNode *XmlTag::convertOr() {
    return TNode::makeOr(nullptr, nullptr);
}

TNode *XmlTag::convertGt() {
    return TNode::makeGt(nullptr, nullptr);
}

TNode *XmlTag::convertGe() {
    return TNode::makeGe(nullptr, nullptr);
}

TNode *XmlTag::convertLt() {
    return TNode::makeLt(nullptr, nullptr);
}

TNode *XmlTag::convertLe() {
    return TNode::makeLe(nullptr, nullptr);
}

TNode *XmlTag::convertEq() {
    return TNode::makeEq(nullptr, nullptr);
}

TNode *XmlTag::convertNe() {
    // will be filled later
    return TNode::makeNe(nullptr, nullptr);
}

TNode *XmlTag::convertVar() {
    ensureKeys({"name"});
    string name = data["name"];
    return TNode::makeVarName(Token::makeName(name));
}

TNode *XmlTag::convertProcName() {
    ensureKeys({"name"});
    string name = data["name"];
    return TNode::makeProcName(Token::makeName(name));
}

TNode *XmlTag::convertConst() {
    ensureKeys({"val"});
    string val = data["val"];
    return TNode::makeConstVal(Token::makeConst(val));
}

TNode *XmlTag::convertPlus() {
    return TNode::makePlus(nullptr, nullptr);
}

TNode *XmlTag::convertMinus() {
    return TNode::makeMinus(nullptr, nullptr);
}

TNode *XmlTag::convertTimes() {
    return TNode::makeTimes(nullptr, nullptr);
}

TNode *XmlTag::convertDiv() {
    return TNode::makeDiv(nullptr, nullptr);
}

TNode *XmlTag::convertMod() {
    return TNode::makeMod(nullptr, nullptr);
}

void XmlTag::ensureKeys(const vector<string> &keys) {
    for (const string &key: keys) {
        if (data.find(key) == data.end()) {
            throw runtime_error("xml tag " + type + " missing key " + key);
        }
    }
}

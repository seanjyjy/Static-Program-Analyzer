#include <utility>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "SimpleParser/Token.h"
#include "TNode.h"

using namespace std;

TNode::TNode(TNodeType type, Token* val, vector<TNode *> children): type(type), val(val), children(move(children)) {}

void TNode::addChild(TNode *child) {
    children.push_back(child);
}

TNode *TNode::getParent() {
    return parent;
}

vector<TNode *> TNode::getChildren() {
    return children;
}

void TNode::setParent(TNode *par) {
    parent = par;
}

void TNode::setLeftChild(TNode *child) {
    children.insert(children.begin(), child);
}

string TNode::toString() {
    if (val != nullptr && !val->getVal().empty()) {
        return TNode::typeToString(type) + " " + val->getVal();
    }
    return TNode::typeToString(type);
}

string TNode::toStringRecursive() {
    string ret;
    stack<pair<TNode*,int>> stk;
    stk.push({ this, 0 });
    while (!stk.empty()) {
        auto [node, spaces] = stk.top();
        stk.pop();
        ret += string(spaces, ' ') + node->toString() + "\n";
        vector<TNode*> ch = node->getChildren();
        reverse(ch.begin(), ch.end()); // left to right dfs
        for (TNode *child: ch) {
            stk.push({ child, spaces+2 });
        }
    }
    return ret;
}

void TNode::printRecursive() {
    cout << toStringRecursive() << endl;
}

TNode *TNode::makeProgram(TNode *procedure) { return new TNode(TNodeType::program, nullptr, { procedure }); }
TNode *TNode::makeProcedure(Token* name, TNode *stmtLst) { return new TNode(TNodeType::procedure, name, { stmtLst }); }
TNode *TNode::makeStmtLst(vector<TNode*> stmts) { return new TNode(TNodeType::stmtLst, nullptr, move(stmts)); }
TNode *TNode::makeReadStmt(TNode *var) { return new TNode(TNodeType::readStmt, nullptr, { var }); }
TNode *TNode::makePrintStmt(TNode *var) { return new TNode(TNodeType::printStmt, nullptr, { var }); }
TNode *TNode::makeCallStmt(TNode *procedure) { return new TNode(TNodeType::callStmt, nullptr, { procedure }); }
TNode *TNode::makeWhileStmt(TNode *condExpr, TNode *stmtLst) { return new TNode(TNodeType::whileStmt, nullptr, { condExpr, stmtLst }); }
TNode *TNode::makeIfStmt(TNode *condExpr, TNode *ifStmtLst, TNode *elseStmtLst) { return new TNode(TNodeType::ifStmt, nullptr, { condExpr, ifStmtLst, elseStmtLst }); }
TNode *TNode::makeAssignStmt(TNode *var, TNode *expr) { return new TNode(TNodeType::assignStmt, nullptr, { var, expr }); }
TNode *TNode::makeNot(TNode *expr) { return new TNode(TNodeType::notOp, nullptr, { expr }); }
TNode *TNode::makeAnd(TNode *lhsCondExpr, TNode *rhsCondExpr) { return new TNode(TNodeType::andOp, nullptr, { lhsCondExpr, rhsCondExpr }); }
TNode *TNode::makeOr(TNode *lhsCondExpr, TNode *rhsCondExpr) { return new TNode(TNodeType::orOp, nullptr, { lhsCondExpr, rhsCondExpr }); }
TNode *TNode::makeGt(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::gt, nullptr, { lhs, rhs }); }
TNode *TNode::makeGe(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::ge, nullptr, { lhs, rhs }); }
TNode *TNode::makeLt(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::lt, nullptr, { lhs, rhs }); }
TNode *TNode::makeLe(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::le, nullptr, { lhs, rhs }); }
TNode *TNode::makeEq(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::eq, nullptr, { lhs, rhs }); }
TNode *TNode::makeNe(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::ne, nullptr, { lhs, rhs }); }
TNode *TNode::makePlus(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::plus, nullptr, { lhs, rhs }); }
TNode *TNode::makeMinus(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::minus, nullptr, { lhs, rhs }); }
TNode *TNode::makeTimes(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::times, nullptr, { lhs, rhs }); }
TNode *TNode::makeDiv(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::div, nullptr, { lhs, rhs }); }
TNode *TNode::makeMod(TNode *lhs, TNode *rhs) { return new TNode(TNodeType::mod, nullptr, { lhs, rhs }); }
TNode *TNode::makeVarName(Token* val) { return new TNode(TNodeType::varName, val, {}); }
TNode *TNode::makeProcName(Token* val) { return new TNode(TNodeType::procName, val, {}); }
TNode *TNode::makeConstVal(Token* val) { return new TNode(TNodeType::constValue, val, {}); }
TNode *TNode::makeDummy() { return new TNode{TNodeType::dummy, nullptr, {}}; }

string TNode::typeToString(TNodeType type) {
    switch (type) {
        case TNodeType::program: return "program";
        case TNodeType::procedure : return "procedure";
        case TNodeType::stmtLst : return "stmtLst";
        case TNodeType::readStmt : return "read";
        case TNodeType::printStmt : return "printRecursive";
        case TNodeType::callStmt : return "call";
        case TNodeType::whileStmt : return "while";
        case TNodeType::ifStmt : return "if";
        case TNodeType::assignStmt : return "=";
        case TNodeType::notOp : return "!";
        case TNodeType::andOp : return "&&";
        case TNodeType::orOp : return "||";
        case TNodeType::gt : return ">";
        case TNodeType::ge : return ">=";
        case TNodeType::lt : return "<";
        case TNodeType::le : return "<=";
        case TNodeType::eq : return "==";
        case TNodeType::ne : return "!=";
        case TNodeType::plus : return "+";
        case TNodeType::minus : return "-";
        case TNodeType::times : return "*";
        case TNodeType::div : return "/";
        case TNodeType::mod : return "%";
        case TNodeType::varName : return "varName";
        case TNodeType::procName : return "procName";
        case TNodeType::constValue : return "constValue";
        case TNodeType::dummy : return "dummy";
        default: throw runtime_error("unknown TNode token type");
    }
}

bool TNode::operator==(TNode &other) {
    // base case: if children are different sizes, not equal
    if (children.size() != other.children.size()) return false;
    // otherwise first compare type and underlying token value (if any)
    bool isEq = true;
    if (val == nullptr && other.val == nullptr) {
        isEq = isEq && (type == other.type);
    } else {
        isEq = isEq && (type == other.type) && (val->getVal() == other.val->getVal());
    }
    // then compare children equality
    for (int i = 0; i < children.size(); i++) {
        isEq = isEq && (*children[i] == *other.children[i]);
    }
    return isEq;
}



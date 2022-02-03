#pragma once

#include <utility>
#include <vector>

#include "Common/TNodeType.h"
#include "SimpleParser/Token.h"

using namespace std;

class TNode {
private:
    TNodeType type;
    Token *val = nullptr; // only leaf nodes and procedure have val
    vector<TNode *> children;
    TNode *parent = nullptr;
public:
    TNode(TNodeType type, Token *val, vector<TNode *> children);

    void addChild(TNode *child);

    void setLeftChild(TNode *child);

    void setAllParents();

    // getters and setters
    TNodeType getType();

    Token *getVal();

    TNode *getParent();

    vector<TNode *> getChildren();

    void setParent(TNode *parent);

    // mainly for debugging purposes
    string toString();

    string toStringRecursive();

    void printRecursive();

    static TNode *makeProgram(TNode *procedure);

    static TNode *makeProcedure(Token *name, TNode *stmtLst);

    static TNode *makeStmtLst(vector<TNode *> stmts); // variable arguments, all TNode*
    static TNode *makeReadStmt(TNode *var);

    static TNode *makePrintStmt(TNode *var);

    static TNode *makeCallStmt(TNode *procedure);

    static TNode *makeWhileStmt(TNode *condExpr, TNode *stmtLst);

    static TNode *makeIfStmt(TNode *condExpr, TNode *ifStmtLst, TNode *elseStmtLst);

    static TNode *makeAssignStmt(TNode *var, TNode *expr);

    static TNode *makeNot(TNode *expr);

    static TNode *makeAnd(TNode *lhsCondExpr, TNode *rhsCondExpr);

    static TNode *makeOr(TNode *lhsCondExpr, TNode *rhsCondExpr);

    static TNode *makeGt(TNode *lhs, TNode *rhs);

    static TNode *makeGe(TNode *lhs, TNode *rhs);

    static TNode *makeLt(TNode *lhs, TNode *rhs);

    static TNode *makeLe(TNode *lhs, TNode *rhs);

    static TNode *makeEq(TNode *lhs, TNode *rhs);

    static TNode *makeNe(TNode *lhs, TNode *rhs);

    static TNode *makePlus(TNode *lhs, TNode *rhs);

    static TNode *makeMinus(TNode *lhs, TNode *rhs);

    static TNode *makeTimes(TNode *lhs, TNode *rhs);

    static TNode *makeDiv(TNode *lhs, TNode *rhs);

    static TNode *makeMod(TNode *lhs, TNode *rhs);

    static TNode *makeVarName(Token *val);

    static TNode *makeProcName(Token *val);

    static TNode *makeConstVal(Token *val);

    static TNode *makeDummy();

    static string typeToString(TNodeType type);
};
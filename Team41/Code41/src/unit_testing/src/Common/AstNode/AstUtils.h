#pragma once

#include "Common/AstNode/TNodeType.h"
#include "SimpleParser/Token.h"
#include "Common/AstNode/Program.h"
#include "Common/AstNode/Procedure.h"
#include "Common/AstNode/StmtLst.h"
#include "Common/AstNode/Read.h"
#include "Common/AstNode/Print.h"
#include "Common/AstNode/Call.h"
#include "Common/AstNode/While.h"
#include "Common/AstNode/If.h"
#include "Common/AstNode/Assign.h"
#include "Common/AstNode/Gt.h"
#include "Common/AstNode/Ge.h"
#include "Common/AstNode/Lt.h"
#include "Common/AstNode/Le.h"
#include "Common/AstNode/Ne.h"
#include "Common/AstNode/Eq.h"
#include "Common/AstNode/VarName.h"
#include "Common/AstNode/ConstVal.h"
#include "Common/AstNode/ProcName.h"
#include "Common/AstNode/Plus.h"
#include "Common/AstNode/Minus.h"
#include "Common/AstNode/Times.h"
#include "Common/AstNode/Div.h"
#include "Common/AstNode/Mod.h"
#include "Common/AstNode/Not.h"
#include "Common/AstNode/And.h"
#include "Common/AstNode/Or.h"
#include "Common/AstNode/CondExpr.h"
#include "Common/AstNode/RelExpr.h"
#include "Common/AstNode/RelFactor.h"

class AstUtils {
public:
    // convenience method to make a program node. Used for testing purposes (including ASTBuilder).
    static Program *makeProgram(const vector<Procedure *>& procedures);

    // convenience method to make a procedure node. Used for testing purposes (including ASTBuilder).
    static Procedure *makeProcedure(Token *name, StmtLst *stmtLst);

    // convenience method to make a statement list node. Used for testing purposes (including ASTBuilder).
    static StmtLst *makeStmtLst(const vector<Stmt *> &stmts);

    // convenience method to make a read node. Used for testing purposes (including ASTBuilder).
    static Read *makeReadStmt(VarName *var);

    // convenience method to make a print node. Used for testing purposes (including ASTBuilder).
    static Print *makePrintStmt(VarName *var);

    // convenience method to make a call node. Used for testing purposes (including ASTBuilder).
    static Call *makeCallStmt(ProcName *procedure);

    // convenience method to make a while node. Used for testing purposes (including ASTBuilder).
    static While *makeWhileStmt(TNode *condExpr, StmtLst *stmtLst);

    // convenience method to make an if node. Used for testing purposes (including ASTBuilder).
    static If *makeIfStmt(TNode *condExpr, StmtLst *ifStmtLst, StmtLst *elseStmtLst);

    // convenience method to make an assignment node. Used for testing purposes (including ASTBuilder).
    static Assign *makeAssignStmt(VarName *var, TNode *expr);

    // convenience method to make a ! node. Used for testing purposes (including ASTBuilder).
    static Not *makeNot(CondExpr *expr);

    // convenience method to make an && node. Used for testing purposes (including ASTBuilder).
    static And *makeAnd(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr);

    // convenience method to make a || node. Used for testing purposes (including ASTBuilder).
    static Or *makeOr(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr);

    // convenience method to make a > node. Used for testing purposes (including ASTBuilder).
    static Gt *makeGt(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a >= node. Used for testing purposes (including ASTBuilder).
    static Ge *makeGe(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a < node. Used for testing purposes (including ASTBuilder).
    static Lt *makeLt(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a <= node. Used for testing purposes (including ASTBuilder).
    static Le *makeLe(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make an == node. Used for testing purposes (including ASTBuilder).
    static Eq *makeEq(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a != node. Used for testing purposes (including ASTBuilder).
    static Ne *makeNe(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a + node. Used for testing purposes (including ASTBuilder).
    static Plus *makePlus(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a - node. Used for testing purposes (including ASTBuilder).
    static Minus *makeMinus(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a * node. Used for testing purposes (including ASTBuilder).
    static Times *makeTimes(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a / node. Used for testing purposes (including ASTBuilder).
    static Div *makeDiv(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a % node. Used for testing purposes (including ASTBuilder).
    static Mod *makeMod(RelFactor *lhs, RelFactor *rhs);

    // convenience method to make a variable node. Used for testing purposes (including ASTBuilder).
    static VarName *makeVarName(Token *val);

    // convenience method to make a process name node. Used for testing purposes (including ASTBuilder).
    static ProcName *makeProcName(Token *val);

    // convenience method to make a constant node. Used for testing purposes (including ASTBuilder).
    static ConstVal *makeConstVal(Token *val);

    static void printAst(TNode* ast);
};

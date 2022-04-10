#include "AstUtils.h"
#include "stack"
#include <iostream>

Program *AstUtils::makeProgram(const vector<Procedure *> &procedures) {
    return new Program(procedures);
}

Procedure *AstUtils::makeProcedure(Token *name, StmtLst *stmtLst) { return new Procedure(name, stmtLst); }

StmtLst *AstUtils::makeStmtLst(const vector<Stmt *> &stmts) { return new StmtLst(stmts); }

Read *AstUtils::makeReadStmt(VarName *var) { return new Read(var); }

Print *AstUtils::makePrintStmt(VarName *var) { return new Print(var); }

Call *AstUtils::makeCallStmt(ProcName *procedure) { return new Call(procedure); }

While *AstUtils::makeWhileStmt(TNode *condExpr, StmtLst *stmtLst) {
    return new While(condExpr, stmtLst);
}

If *AstUtils::makeIfStmt(TNode *condExpr, StmtLst *ifStmtLst, StmtLst *elseStmtLst) {
    return new If(condExpr, ifStmtLst, elseStmtLst);
}

Assign *AstUtils::makeAssignStmt(VarName *var, TNode *expr) { return new Assign(var, expr); }

Not *AstUtils::makeNot(CondExpr *expr) { return new Not(expr); }

And *AstUtils::makeAnd(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr) {
    return new And(lhsCondExpr, rhsCondExpr);
}

Or *AstUtils::makeOr(CondExpr *lhsCondExpr, CondExpr *rhsCondExpr) {
    return new Or(lhsCondExpr, rhsCondExpr);
}

Gt *AstUtils::makeGt(RelFactor *lhs, RelFactor *rhs) { return new Gt(lhs, rhs); }

Ge *AstUtils::makeGe(RelFactor *lhs, RelFactor *rhs) { return new Ge(lhs, rhs); }

Lt *AstUtils::makeLt(RelFactor *lhs, RelFactor *rhs) { return new Lt(lhs, rhs); }

Le *AstUtils::makeLe(RelFactor *lhs, RelFactor *rhs) { return new Le(lhs, rhs); }

Eq *AstUtils::makeEq(RelFactor *lhs, RelFactor *rhs) { return new Eq(lhs, rhs); }

Ne *AstUtils::makeNe(RelFactor *lhs, RelFactor *rhs) { return new Ne(lhs, rhs); }

Plus *AstUtils::makePlus(RelFactor *lhs, RelFactor *rhs) { return new Plus(lhs, rhs); }

Minus *AstUtils::makeMinus(RelFactor *lhs, RelFactor *rhs) { return new Minus(lhs, rhs); }

Times *AstUtils::makeTimes(RelFactor *lhs, RelFactor *rhs) { return new Times(lhs, rhs); }

Div *AstUtils::makeDiv(RelFactor *lhs, RelFactor *rhs) { return new Div(lhs, rhs); }

Mod *AstUtils::makeMod(RelFactor *lhs, RelFactor *rhs) { return new Mod(lhs, rhs); }

VarName *AstUtils::makeVarName(Token *val) { return new VarName(val); }

ProcName *AstUtils::makeProcName(Token *val) { return new ProcName(val); }

ConstVal *AstUtils::makeConstVal(Token *val) { return new ConstVal(val); }

void AstUtils::printAst(TNode *ast) {
    string ret;
    stack<pair<TNode *, int>> stk;
    stk.push({ast, 0});
    while (!stk.empty()) {
        auto[node, spaces] = stk.top();
        stk.pop();
        ret += string(spaces, ' ') + node->toString() + "\n";
        vector<TNode *> ch = node->getChildren();
        reverse(ch.begin(), ch.end()); // left to right dfs
        for (TNode *child: ch) {
            stk.push({child, spaces + 2});
        }
    }
    cout << ret << endl;
}

#pragma once

#include <utility>
#include <vector>

#include "Common/TNodeType.h"
#include "SimpleParser/Token.h"

using namespace std;

class TNode {
 private:
  TNodeType type; // ast node type, e.g "program"
  Token *val = nullptr; // only leaf nodes and procedure have val
  vector<TNode *> children; // order matters
  TNode *parent = nullptr; // will be set after ast is parsed
 public:
  TNode(TNodeType type, Token *val, vector<TNode *> children);

  ~TNode();

  // appends a child to rightmost end of children.
  void addChild(TNode *child);

  // prepends a child to leftmost end, pushing all children right.
  void setLeftChild(TNode *child);

  // performs recursive backtracking and sets all parent pointers.
  void setAllParents();

  // getters
  TNodeType getType();

  Token *getVal();

  TNode *getParent();

  vector<TNode *> getChildren();

  string getTokenVal();

  // setters
  void setParent(TNode *parent);

  void setChildren(vector<TNode *> children);

  // returns a human-readable string representation of the current ast node, for debugging purposes.
  string toString();

  // returns a human-readable string representation of the entire tree, for debugging purposes.
  string toStringRecursive();

  // convenience method that calls toStringRecursive() and prints to cout, for debugging purposes.
  void printRecursive();

  // convenience method to make a program node. Used for testing purposes (including ASTBuilder).
  static TNode *makeProgram(vector<TNode *> procedures);

  // convenience method to make a procedure node. Used for testing purposes (including ASTBuilder).
  static TNode *makeProcedure(Token *name, TNode *stmtLst);

  // convenience method to make a statement list node. Used for testing purposes (including ASTBuilder).
  static TNode *makeStmtLst(vector<TNode *> stmts);

  // convenience method to make a read node. Used for testing purposes (including ASTBuilder).
  static TNode *makeReadStmt(TNode *var);

  // convenience method to make a print node. Used for testing purposes (including ASTBuilder).
  static TNode *makePrintStmt(TNode *var);

  // convenience method to make a call node. Used for testing purposes (including ASTBuilder).
  static TNode *makeCallStmt(TNode *procedure);

  // convenience method to make a while node. Used for testing purposes (including ASTBuilder).
  static TNode *makeWhileStmt(TNode *condExpr, TNode *stmtLst);

  // convenience method to make an if node. Used for testing purposes (including ASTBuilder).
  static TNode *makeIfStmt(TNode *condExpr, TNode *ifStmtLst, TNode *elseStmtLst);

  // convenience method to make an assignment node. Used for testing purposes (including ASTBuilder).
  static TNode *makeAssignStmt(TNode *var, TNode *expr);

  // convenience method to make a ! node. Used for testing purposes (including ASTBuilder).
  static TNode *makeNot(TNode *expr);

  // convenience method to make an && node. Used for testing purposes (including ASTBuilder).
  static TNode *makeAnd(TNode *lhsCondExpr, TNode *rhsCondExpr);

  // convenience method to make a || node. Used for testing purposes (including ASTBuilder).
  static TNode *makeOr(TNode *lhsCondExpr, TNode *rhsCondExpr);

  // convenience method to make a > node. Used for testing purposes (including ASTBuilder).
  static TNode *makeGt(TNode *lhs, TNode *rhs);

  // convenience method to make a >= node. Used for testing purposes (including ASTBuilder).
  static TNode *makeGe(TNode *lhs, TNode *rhs);

  // convenience method to make a < node. Used for testing purposes (including ASTBuilder).
  static TNode *makeLt(TNode *lhs, TNode *rhs);

  // convenience method to make a <= node. Used for testing purposes (including ASTBuilder).
  static TNode *makeLe(TNode *lhs, TNode *rhs);

  // convenience method to make an == node. Used for testing purposes (including ASTBuilder).
  static TNode *makeEq(TNode *lhs, TNode *rhs);

  // convenience method to make a != node. Used for testing purposes (including ASTBuilder).
  static TNode *makeNe(TNode *lhs, TNode *rhs);

  // convenience method to make a + node. Used for testing purposes (including ASTBuilder).
  static TNode *makePlus(TNode *lhs, TNode *rhs);

  // convenience method to make a - node. Used for testing purposes (including ASTBuilder).
  static TNode *makeMinus(TNode *lhs, TNode *rhs);

  // convenience method to make a * node. Used for testing purposes (including ASTBuilder).
  static TNode *makeTimes(TNode *lhs, TNode *rhs);

  // convenience method to make a / node. Used for testing purposes (including ASTBuilder).
  static TNode *makeDiv(TNode *lhs, TNode *rhs);

  // convenience method to make a % node. Used for testing purposes (including ASTBuilder).
  static TNode *makeMod(TNode *lhs, TNode *rhs);

  // convenience method to make a variable node. Used for testing purposes (including ASTBuilder).
  static TNode *makeVarName(Token *val);

  // convenience method to make a process name node. Used for testing purposes (including ASTBuilder).
  static TNode *makeProcName(Token *val);

  // convenience method to make a constant node. Used for testing purposes (including ASTBuilder).
  static TNode *makeConstVal(Token *val);

  // returns a human-readable representation of the given type.
  static string typeToString(TNodeType type);
};
#pragma once

#include <utility>
#include <vector>

#include "TNodeType.h"
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

    // returns a human-readable string representation of the current ast node, for debugging purposes.
    virtual string toString();

    // convenience functions
    [[nodiscard]] virtual bool isProgram() const;
    [[nodiscard]] virtual bool isProcedure() const;
    [[nodiscard]] virtual bool isStmtLst() const;
    [[nodiscard]] virtual bool isRead() const;
    [[nodiscard]] virtual bool isPrint() const;
    [[nodiscard]] virtual bool isCall() const;
    [[nodiscard]] virtual bool isWhile() const;
    [[nodiscard]] virtual bool isIf() const;
    [[nodiscard]] virtual bool isAssign() const;
    [[nodiscard]] virtual bool isStmt() const;
    [[nodiscard]] virtual bool isNot() const;
    [[nodiscard]] virtual bool isAnd() const;
    [[nodiscard]] virtual bool isOr() const;
    [[nodiscard]] virtual bool isGt() const;
    [[nodiscard]] virtual bool isGe() const;
    [[nodiscard]] virtual bool isLt() const;
    [[nodiscard]] virtual bool isLe() const;
    [[nodiscard]] virtual bool isEq() const;
    [[nodiscard]] virtual bool isNe() const;
    [[nodiscard]] virtual bool isPlus() const;
    [[nodiscard]] virtual bool isMinus() const;
    [[nodiscard]] virtual bool isTimes() const;
    [[nodiscard]] virtual bool isDiv() const;
    [[nodiscard]] virtual bool isMod() const;
    [[nodiscard]] virtual bool isArithmeticOp() const;
    [[nodiscard]] virtual bool isCondExpr() const;
    [[nodiscard]] virtual bool isVarName() const;
    [[nodiscard]] virtual bool isProcName() const;
    [[nodiscard]] virtual bool isConstVal() const;

    // getters
    TNodeType getType();

    Token *getVal();

    TNode *getParent();

    // getters and setters
    const vector<TNode *> &getChildren();

    string getTokenVal();

    void setParent(TNode *parent);

    void setChildren(vector<TNode *> children);
};
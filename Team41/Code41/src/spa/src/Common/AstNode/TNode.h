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
    virtual bool isProgram() const;
    virtual bool isProcedure() const;
    virtual bool isStmtLst() const;
    virtual bool isRead() const;
    virtual bool isPrint() const;
    virtual bool isCall() const;
    virtual bool isWhile() const;
    virtual bool isIf() const;
    virtual bool isAssign() const;
    virtual bool isStmt() const;
    virtual bool isNot() const;
    virtual bool isAnd() const;
    virtual bool isOr() const;
    virtual bool isGt() const;
    virtual bool isGe() const;
    virtual bool isLt() const;
    virtual bool isLe() const;
    virtual bool isEq() const;
    virtual bool isNe() const;
    virtual bool isPlus() const;
    virtual bool isMinus() const;
    virtual bool isTimes() const;
    virtual bool isDiv() const;
    virtual bool isMod() const;
    virtual bool isVarName() const;
    virtual bool isProcName() const;
    virtual bool isConstVal() const;

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
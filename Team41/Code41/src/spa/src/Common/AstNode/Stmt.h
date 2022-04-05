#pragma once

#include "TNode.h"
#include "TNodeType.h"

/**
 * Represents stmt -> read | print | call | while | if | assign
 */
class Stmt : public TNode {
private:
    static TNodeType verify(TNodeType type);
public:
    Stmt(TNodeType type, vector<TNode *> children);
    string toString() override;
    [[nodiscard]] bool isStmt() const override;
};

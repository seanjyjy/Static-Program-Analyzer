#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/VarName.h"

/**
 * Represents read -> 'read' var_name ';'
 */
class Read : public Stmt {
public:
    Read(VarName *var);
    string toString() override;
    [[nodiscard]] bool isRead() const override;
};

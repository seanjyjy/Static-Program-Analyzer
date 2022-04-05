#pragma once

#include "TNode.h"
#include "Common/AstNode/Stmt.h"
#include "Common/AstNode/ProcName.h"

/**
 * Represents call -> 'call' proc_name ';'.
 */
class Call : public Stmt {
public:
    Call(ProcName *procedure);
    string toString() override;
    [[nodiscard]] bool isCall() const override;
};

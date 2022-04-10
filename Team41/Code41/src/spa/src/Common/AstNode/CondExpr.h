#pragma once

#include "TNode.h"
#include "TNodeType.h"
#include <stdexcept>
#include <utility>

using namespace std;

/**
 * Represents cond_expr  -> rel_expr
							| '!' '(' cond_expr ')'
							| '(' cond_expr ')' '&&' '(' cond_expr ')'
							| '(' cond_expr ')' '||' '(' cond_expr ')'
 */
class CondExpr : public TNode {
private:
    static TNodeType verify(TNodeType type);

public:
    CondExpr(TNodeType type, vector<TNode *> children);

    string toString() override;

    [[nodiscard]] bool isCondExpr() const override;
};

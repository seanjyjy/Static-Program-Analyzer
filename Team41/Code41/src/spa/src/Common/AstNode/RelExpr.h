#pragma once

#include "Common/AstNode/CondExpr.h"
#include "Common/AstNode/RelFactor.h"
#include "TNodeType.h"
#include <stdexcept>
#include <utility>

using namespace std;

/**
 * Represents rel_expr   -> rel_factor '>' rel_factor
							| rel_factor '>=' rel_factor
							| rel_factor '<' rel_factor
							| rel_factor '<=' rel_factor
							| rel_factor '==' rel_factor
							| rel_factor '!=' rel_factor
 */
class RelExpr : public CondExpr {
private:
    static TNodeType verify(TNodeType type);

public:
    RelExpr(TNodeType type, RelFactor *lhs, RelFactor *rhs);

    string toString() override;
};

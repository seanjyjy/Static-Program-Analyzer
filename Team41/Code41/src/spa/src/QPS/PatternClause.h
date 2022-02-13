#pragma once

#include "Common/TNode.h"
#include "ClauseVariable.h"
#include "QueryDeclaration.h"
#include "PatternVariable.h"

class PatternClause {
private:
    QueryDeclaration synonym;
    ClauseVariable lhs;
    PatternVariable rhs; // miniAST exists within

public:
    QueryDeclaration getSynonym() const;
    ClauseVariable getLHS() const;
    PatternVariable getRHS() const;
    PatternClause(QueryDeclaration synonym, ClauseVariable lhs, PatternVariable rhs);
};

// todo: Be wary of advanced spa extensions. pattern will support more than just assignment

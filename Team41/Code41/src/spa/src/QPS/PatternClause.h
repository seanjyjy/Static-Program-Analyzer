#pragma once

#include "Common/TNode.h"
#include "ClauseVariable.h"
#include "QueryDeclaration.h"

class PatternClause {
private:
    QueryDeclaration synonym;
    ClauseVariable lhs;
    TNode *miniAST;
    enum pattern_type {
        wildcard, fullpattern, subpattern
    };
    pattern_type type;
public:
    QueryDeclaration getSynonym() const;
    ClauseVariable getLHS() const;
    TNode *getRHS() const;
    void setIsWildcard();
    void setIsFullPattern();
    void setIsSubPattern();
    bool isWildcard();
    bool isFullPattern();
    bool isSubPattern();
    PatternClause(QueryDeclaration synonym, ClauseVariable lhs, TNode *miniAST);
};

// todo: Be wary of advanced spa extensions. pattern will support more than just assignment

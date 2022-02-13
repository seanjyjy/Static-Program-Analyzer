#pragma once

#include "QueryClause.h"

class PatternClause : public QueryClause {
private:
    QueryDeclaration synonym;
public:
    QueryDeclaration getSynonym() const;
    PatternClause(clause_type type, ClauseVariable left, ClauseVariable right, QueryDeclaration synonym);
};

// todo: Be wary of advanced spa extensions. pattern will support more than just assignment

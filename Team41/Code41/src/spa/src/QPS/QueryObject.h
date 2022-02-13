#pragma once

#include <string>
#include <vector>
#include "QueryDeclaration.h"
#include "QueryClause.h"
#include "PatternClause.h"

using namespace std;

class QueryObject {
public:
    vector<QueryDeclaration> declarations;
    vector<QueryClause> clauses;
    vector<PatternClause> patternClauses;
    QueryDeclaration selectSynonym;
    bool isQueryValid;
    QueryObject(vector<QueryDeclaration> declarations, vector<QueryClause> clauses, vector<PatternClause> patternClauses, QueryDeclaration selectSynonym, bool isQueryValid);
};

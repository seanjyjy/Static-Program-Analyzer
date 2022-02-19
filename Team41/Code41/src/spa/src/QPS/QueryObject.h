#pragma once

#include <string>
#include <vector>
#include "QueryDeclaration.h"
#include "QueryClause.h"
#include "PatternClause.h"

using namespace std;

class QueryObject {
public:
    vector<QueryDeclaration> declarations; // collection of declarations in the query
    vector<QueryClause> clauses; // collection of clauses made during the query
    vector<PatternClause> patternClauses; // collection of "pattern" clauses
    QueryDeclaration selectSynonym; // synonym to select for the query
    bool isQueryValid; // indicates if query was valid

    /**
     * Constructor for the QueryObject class.
     *
     * @param declarations vector storing declared entities.
     * @param clauses vector storing query clauses.
     * @param patternClauses vector storing "pattern" clauses.
     * @param selectSynonym synonym to select for the query.
     * @param isQueryValid query's validity.
     */
    QueryObject(vector<QueryDeclaration> declarations, vector<QueryClause> clauses, vector<PatternClause> patternClauses, QueryDeclaration selectSynonym, bool isQueryValid);
    ~QueryObject();
};

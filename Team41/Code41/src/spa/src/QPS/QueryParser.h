#pragma once

#include <unordered_set>

#include "QueryObject.h"
#include "QueryDeclaration.h"
#include "QueryEvaluator.h"
#include "QueryLexer.h"

using namespace std;

class QueryParser {
private:
    const string &input;
    QueryObject *queryObject;
    QueryLexer *lex;

    bool skipSuchThat();
    bool isQueryClauseValid(string type, string left, string right);
    QueryClause::clause_type determineClauseType(string type, string left, string right);
    ClauseVariable::variable_type determineVariableType(string w);
    bool parseDeclarations();
    bool parseSelectSynonym();
    bool parseClause();
    bool parsePatternClause();
    bool buildClause(string clause, string left, string right);
    bool isDeclared(string synonym);
    bool isDeclaredProcedure(string synonym);

    optional<QueryDeclaration> findMatchingDeclaration(string synonym);
public:
    QueryParser(string &input);
    QueryObject *parse();
};

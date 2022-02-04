#pragma once

#include <unordered_set>

#include "QueryObject.h"
#include "QueryDeclaration.h"
#include "QueryEvaluator.h"

using namespace std;

class QueryParser {
private:
    const string &input;
    const unordered_set<string> declaration_keywords =
            {"stmt", "read", "print", "call", "while",
             "if", "assign", "variable", "constant", "procedure"};
    int index = 0;
    char currChar;
    bool declarationEndFound = false;
    QueryObject *queryObject;

    string &readType();
    string &readSynonym();
    int skipToNearestChar();
    bool skipSuchThat();
    bool parseDeclarations();
    bool parseSelectSynonym();
public:
    QueryParser(string &input);
    QueryObject *parse();
};

#include <stdio.h>

#include "QueryParser.h"

using namespace std;

// todo: check if valid declaration type

string &QueryParser::readType() {
    string type = "";
    do {
        currChar = input.at(index);
        type.push_back(currChar);
        index++;
    } while (!isspace(input.at(index)));
    index++; // Skip over space.

    // If we stumble upon the "Select" keyword, no more declarations.
    declarationEndFound = type == "Select";

    return type;
}

// todo: check for invalid characters. e.g. Can't have '*' as part of the synonym right?

string &QueryParser::readSynonym() {
    string synonym = "";
    do {
        currChar = input.at(index);
        if (isspace(currChar)) {
            // Invalid. No spaces in synonym.
            printf("Syntax Error: Invalid synonym.");
        }
        synonym.push_back(currChar);
        index++;
    } while (input.at(index) != ';');
    index++; // Skip over ';'
    return synonym;
}

int QueryParser::skipToNearestChar() {
    while(isspace(input.at(index))) { // todo: determine if skipping spaces alone is sufficient. May have edge cases.
        index++;
    }
    return 0;
}

bool QueryParser::skipSuchThat() {
    int st_length = 9;
    string s = input.substr(index, st_length);
    if (s == "such that") {
        index += st_length;
    }
}

bool QueryParser::parseDeclarations() {
    do {
        string firstType = readType();
        if (declarationEndFound) break; // todo: see if there's a better way.
        printf("<%s> ", firstType.c_str());
        string firstSyn = readSynonym();
        printf("<%s>\n", firstSyn.c_str());
        skipToNearestChar();
    } while (!declarationEndFound); // We keep looking for declarations until we see the "Select" keyword.

    skipToNearestChar();

    // todo: Actually populate the QueryObject's declarations vector

    return true;
}

bool QueryParser::parseSelectSynonym() {
    string synonym = "";

    do {
        currChar = input.at(index);
        synonym.push_back(currChar);
        index++;
    } while (!isspace(input.at(index)));
    index++; // Skip over space.
    skipToNearestChar();

    printf("Select <%s>\n", synonym.c_str());

    // todo: populate the QueryObject's select synonym. As a QueryDeclaration?
}

bool QueryParser::parseClause() {
    string clause, var1, var2 = "";
    do {
        currChar = input.at(index);
        clause.push_back(currChar);
        index++;
    } while (input.at(index) != '(');

    // todo: handle malformed inputs. What if no '(' even show up.
    index++; // go over '('
    do {
        currChar = input.at(index);
        var1.push_back(currChar);
        index++;
    } while (input.at(index) != ',');
    index++; // go over ','
    skipToNearestChar();
    do {
        currChar = input.at(index);
        var2.push_back(currChar);
        index++;
    } while (input.at(index) != ')');
    index++; // go over ')'

    printf("<%s> <%s> <%s>\n", clause.c_str(), var1.c_str(), var2.c_str());

    // todo: populate the QueryObject's Clause Object

    return true;
}

bool QueryParser::parsePatternClause() {
    return true;
}

QueryParser::QueryParser(string &input) : input(input) {}

QueryObject *QueryParser::parse() {
    vector<QueryDeclaration> declarations;
    vector<QueryClause> clauses;
    queryObject = new QueryObject(declarations, clauses, false);

    parseDeclarations();
    parseSelectSynonym();

    // todo: Note that queries can end here. Check if ended

    skipSuchThat();
    skipToNearestChar();
    parseClause();

    // todo: Note that queries can end here. Check if ended

    parsePatternClause();
}

// todo: There's some repeated code patterns here and there. Try to optimize later.
// todo: implementation is not proofed against malformed inputs yet.

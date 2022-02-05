#pragma once

#include <string>

using namespace std;

class QueryDeclaration {
public:
    enum design_entity_type {
        STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE
    };
    design_entity_type type;
    string synonym;

    QueryDeclaration(design_entity_type type, string &synonym);
};

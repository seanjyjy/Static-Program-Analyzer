#pragma once

#include <string>

using namespace std;

class QueryDeclaration {
public:
    enum design_entity_type {
        stmt, read, print, call, _while, _if, assign, variable, constant, procedure
    };
    design_entity_type type;
    string synonym;

    QueryDeclaration(design_entity_type type, string &synonym);
};

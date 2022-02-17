#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

class QueryDeclaration {
public:
    // enum representing all possible declaration types in PQL
    enum design_entity_type {
        STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN, VARIABLE, CONSTANT, PROCEDURE, NONE
    };

    // declaration type of the declaration instance
    design_entity_type type;

    // accompanying synonym of a declaration
    string synonym;

    /**
     * Constructor for the QueryDeclaration class.
     *
     * @param type of the new declaration.
     * @param synonym string accompanying the new declaration.
     */
    QueryDeclaration(design_entity_type type, string &synonym);

    /**
     * Converts a string representation of the declaration type
     * to the system representation using the enum.
     *
     * @param s string representing the declaration type.
     * @return design_entity_type corressponding to the declaration.
     */
    static design_entity_type stringToType(string &s);
};

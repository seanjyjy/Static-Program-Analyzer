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

    // todo: make private after all refactor
    // declaration type of the declaration instance
    design_entity_type type;

    // todo: make private after all refactor
    // accompanying synonym of a declaration
    string synonym;

    design_entity_type getType();

    string getSynonym();

    QueryDeclaration();

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
     * @return design_entity_type corresponding to the declaration.
     */
    static design_entity_type stringToType(string &s);

    /**
     * Converts the enum design entity type into a human-readable string representation.
     *
     * @param d a valid design entity type as specified by the enum.
     * @return a human-readable string representation.
     * @throws runtime_error if the given design entity type has no string mapping.
     */
    static string typeToString(design_entity_type &d);

    /**
     * Returns a human-readable string representation of the query declaration.
     */
    string toString();

    /**
     * Prints the query declaration's type and synonym in a human-readable form.
     */
    void print();
};

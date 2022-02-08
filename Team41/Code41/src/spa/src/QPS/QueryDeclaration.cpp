#include "QueryDeclaration.h"

QueryDeclaration::QueryDeclaration(design_entity_type type, string &synonym) :
    type(type), synonym(synonym) {}

QueryDeclaration::design_entity_type QueryDeclaration::stringToType(string &s) {
    if (s == "stmt")
        return QueryDeclaration::STMT;
    if (s == "read")
        return QueryDeclaration::READ;
    if (s == "print")
        return QueryDeclaration::PRINT;
    if (s == "call")
        return QueryDeclaration::CALL;
    if (s == "while")
        return QueryDeclaration::WHILE;
    if (s == "if")
        return QueryDeclaration::IF;
    if (s == "assign")
        return QueryDeclaration::ASSIGN;
    if (s == "variable")
        return QueryDeclaration::VARIABLE;
    if (s == "constant")
        return QueryDeclaration::CONSTANT;
    if (s == "procedure")
        return QueryDeclaration::PROCEDURE;
}


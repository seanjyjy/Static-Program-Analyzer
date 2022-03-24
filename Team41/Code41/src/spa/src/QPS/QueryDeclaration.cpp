#include "QueryDeclaration.h"

QueryDeclaration::QueryDeclaration() {
    type = NONE;
}

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
    throw runtime_error("no query declaration for string " + s);
}

string QueryDeclaration::typeToString(QueryDeclaration::design_entity_type &d) {
    if (d == QueryDeclaration::STMT) return "stmt";
    if (d == QueryDeclaration::READ) return "read";
    if (d == QueryDeclaration::PRINT) return "print";
    if (d == QueryDeclaration::CALL) return "call";
    if (d == QueryDeclaration::WHILE) return "while";
    if (d == QueryDeclaration::IF) return "if";
    if (d == QueryDeclaration::ASSIGN) return "assign";
    if (d == QueryDeclaration::VARIABLE) return "variable";
    if (d == QueryDeclaration::CONSTANT) return "constant";
    if (d == QueryDeclaration::PROCEDURE) return "procedure";
    throw runtime_error("no string representation for design entity");
}

string QueryDeclaration::toString() {
    return typeToString(type) + " " + synonym;
}

void QueryDeclaration::print() {
    cout << toString() << endl;
}

bool QueryDeclaration::equals(QueryDeclaration other) const {
    if (type != other.getType())
        return false;
    if (synonym != other.getSynonym())
        return false;

    return true;
}


#include "QueryDeclaration.h"

#include <utility>

QueryDeclaration::QueryDeclaration() : type(NoneEntities::getInstance()) {}

QueryDeclaration::QueryDeclaration(Entities *type, string &synonym) :
    type(type), synonym(synonym) {}

Entities* QueryDeclaration::stringToType(string &s) {
    if (s == "stmt")
        return new StmtEntities();
    if (s == "read")
        return new ReadEntities();
    if (s == "print")
        return new PrintEntities();
    if (s == "call")
        return new CallEntities();
    if (s == "while")
        return new WhileEntities();
    if (s == "if")
        return new IfEntities();
    if (s == "assign")
        return new AssignEntities();
    if (s == "variable")
        return new VariableEntities();
    if (s == "constant")
        return new ConstantEntities();
    if (s == "procedure")
        return new ProcedureEntities();
    throw runtime_error("no query declaration for string " + s);
}

string QueryDeclaration::typeToString(Entities *d) {
    if (d->isNone()) {
        throw runtime_error("no string representation for design entity");
    }

    return d->toString();
}

string QueryDeclaration::toString() {
    return typeToString(type) + " " + synonym;
}

void QueryDeclaration::print() {
    cout << toString() << endl;
}

bool QueryDeclaration::equals(QueryDeclaration other) const {
    if (type->hashCode() != other.getType()->hashCode())
        return false;
    if (synonym != other.getSynonym())
        return false;

    return true;
}

void QueryDeclaration::cleanUp() {
    delete type;
}
#pragma once

#include "QueryDeclaration.h"
#include <string>

using namespace std;

class WithVariable {
public:
    enum withRefType {
        IDENT, INTEGER, ATTR_REF
    };
    enum attributeName {
        PROC_NAME, VAR_NAME, VALUE, STMT_NUM, NONE
    };

    WithVariable(int);
    WithVariable(string);
    WithVariable(attributeName, QueryDeclaration);
    withRefType getType();
    attributeName getAttr();
    QueryDeclaration getSynonym();
    string getIdent();
    int getInteger();
    string getIntegerAsString();

private:
    withRefType type;
    attributeName attr;
    QueryDeclaration synonym;
    string ident;
    int integer;
};


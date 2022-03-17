#pragma once

#include "QueryDeclaration.h"

class WithVariable {
public:
    enum withRefType {
        IDENT, INTEGER, ATTR_REF
    };
    enum attributeName {
        PROC_NAME, VAR_NAME, VALUE, STMT_NUM, NONE
    };
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


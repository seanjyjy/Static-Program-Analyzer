#pragma once

#include "QPS/QO/QueryDeclaration.h"
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

    WithVariable();
//    WithVariable(int);
//    WithVariable(string);
    WithVariable(string, withRefType);
    WithVariable(attributeName, QueryDeclaration);
    withRefType getType() const;
    attributeName getAttr() const;
    QueryDeclaration getSynonym() const;
    string getIdent() const;
    string getInteger() const;
    string getIntegerAsString() const;

    bool isIdentifier() const;
    bool isInteger() const;
    bool isAttrRef() const;

    bool equals(WithVariable) const;
    string toString() const;
private:
    withRefType type;
    attributeName attr;
    QueryDeclaration synonym;
    string ident;
    string integer;
};


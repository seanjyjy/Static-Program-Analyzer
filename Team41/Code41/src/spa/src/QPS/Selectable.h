#pragma once

#include "QueryDeclaration.h"

class Selectable {
public:
    enum attributeName {
        PROC_NAME, VAR_NAME, VALUE, STMT_NUM, NONE
    };
    enum selectableType {
        SYNONYM, ATTR_REF
    };

    attributeName getAttr();

    selectableType getType();

    QueryDeclaration getSynonym();

    Selectable(selectableType type, QueryDeclaration synonym, attributeName attr);

private:
    selectableType type;
    attributeName attr;
    QueryDeclaration synonym;
};

// SYNONYM : IDENT
// ATTR_REF: synonym '.' attrName


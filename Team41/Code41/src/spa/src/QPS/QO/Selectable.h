#pragma once

#include "QPS/QO/QueryDeclaration.h"

class Selectable {
public:
    enum attributeName {
        PROC_NAME, VAR_NAME, VALUE, STMT_NUM, NONE
    };
    enum selectableType {
        SYNONYM, ATTR_REF
    };

    /**
     * Getter for the attribute of the Selectable.
     *
     * @return attributeName representing the selected attribute.
     */
    attributeName getAttr() const;

    /**
     * Getter for the type of the Selectable.
     *
     * @return selectableType representing the selectable's type.
     */
    selectableType getType() const;

    /**
     * Getter for the synonym of the Selectable.
     *
     * @return QueryDeclaration representing the selectable's synonym.
     */
    QueryDeclaration getSynonym() const;

    /**
     * Getter for the synonym name of the Selectable.
     *
     * @return string representing the selectable's synonym name.
     */
    [[nodiscard]] string getSynonymName() const;

    Selectable(selectableType type, QueryDeclaration synonym, attributeName attr);

private:
    selectableType type;
    attributeName attr;
    QueryDeclaration synonym;
};

// SYNONYM : IDENT
// ATTR_REF: synonym '.' attrName

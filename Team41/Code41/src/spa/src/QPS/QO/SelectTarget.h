#pragma once

#include <vector>

#include "Selectable.h"

using namespace std;

class SelectTarget {
public:
    enum targetType {
        BOOLEAN, TUPLE
    };

    /**
     * Checks if the select target is a boolean.
     *
     * @return boolean indicating if the select target is a boolean.
     */
    bool isBoolean() const;

    /**
     * Checks if the select target is a tuple.
     *
     * @return boolean indicating if the select target is a tuple.
     */
    bool isTuple() const;

    /**
     * Adds a selectable to the select target's tuple.
     *
     * @param newSelectable to be added into the tuple.
     */
    void addSelectable(Selectable newSelectable);

    /**
     * Setter for the type of the select target.
     *
     * @param type representing the select target's type.
     */
    void setType(targetType type);

    SelectTarget(targetType type);

    /**
     * Getter for the tuple of selectables.
     *
     * @return vector of Selectables representing the select target's tuple.
     */
    vector<Selectable> getSelectable() const;

private:
    targetType type;
    vector<Selectable> tuple;
};

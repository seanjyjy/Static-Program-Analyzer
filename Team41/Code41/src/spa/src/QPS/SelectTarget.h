#pragma once

#include <vector>

#include "Selectable.h"

using namespace std;

class SelectTarget {
public:
    enum targetType {
        BOOLEAN, TUPLE
    };

    bool isBoolean();

    bool isTuple();

    void addSelectable(Selectable newSelectable);

    SelectTarget(targetType type);

    vector<Selectable> tuple;

private:
    targetType type;
};

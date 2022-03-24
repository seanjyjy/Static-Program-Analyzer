#pragma once

#include <vector>

#include "Selectable.h"

using namespace std;

class SelectTarget {
public:
    enum targetType {
        BOOLEAN, TUPLE
    };

    bool isBoolean() const;

    bool isTuple() const;

    void addSelectable(Selectable newSelectable);

    void setType(targetType type);

    SelectTarget(targetType type);

    vector<Selectable> getSelectable() const;

private:
    targetType type;
    vector<Selectable> tuple;
};

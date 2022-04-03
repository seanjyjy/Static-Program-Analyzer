#pragma once

#include "Entities.h"

class NoneEntities : public Entities {
public:
    NoneEntities();

    ~NoneEntities() override;

    bool isNone() override;

    string toString() override;

    int hashCode() override;

    static NoneEntities* getInstance();
};
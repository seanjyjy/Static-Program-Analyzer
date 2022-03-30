#pragma once

#include "Entities.h"

class NoneEntities : public Entities {
public:
    NoneEntities();

    ~NoneEntities() override;

    bool isNone() override;

    EntitiesReader *getReader() override;

    string toString() override;

    int hashCode() override;
};
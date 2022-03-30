#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/VariableReader.h"

class VariableEntities : public Entities {
public:
    VariableEntities();

    ~VariableEntities() override;

    bool isVariable() override;

    EntitiesReader *getReader() override;

    string toString() override;

    int hashCode() override;
};

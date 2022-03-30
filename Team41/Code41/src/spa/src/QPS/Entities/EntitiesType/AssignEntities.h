#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/AssignReader.h"

class AssignEntities : public Entities {
public:
    AssignEntities();

    ~AssignEntities() override;

    bool isAssign() override;

    EntitiesReader *getReader() override;

    string toString() override;

    int hashCode() override;
};


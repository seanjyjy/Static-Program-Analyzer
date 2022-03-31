#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/IfReader.h"

class IfEntities : public Entities {
public:
    IfEntities();

    ~IfEntities() override;

    bool isIf() override;

    string toString() override;

    int hashCode() override;
};

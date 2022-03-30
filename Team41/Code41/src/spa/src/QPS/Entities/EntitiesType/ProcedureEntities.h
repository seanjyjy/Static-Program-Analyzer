#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/ProcedureReader.h"

class ProcedureEntities : public Entities {
public:
    ProcedureEntities();

    ~ProcedureEntities() override;

    bool isProcedure() override;

    string toString() override;

    int hashCode() override;
};
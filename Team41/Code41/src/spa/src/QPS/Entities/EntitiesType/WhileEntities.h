#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/WhileReader.h"

class WhileEntities : public Entities {
public:
    WhileEntities();

    ~WhileEntities() override;

    bool isWhile() override;

    string toString() override;

    int hashCode() override;
};

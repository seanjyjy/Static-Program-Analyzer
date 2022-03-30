#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/StmtReader.h"

class StmtEntities : public Entities {
public:
    StmtEntities();

    ~StmtEntities() override;

    bool isStmt() override;

    string toString() override;

    int hashCode() override;
};

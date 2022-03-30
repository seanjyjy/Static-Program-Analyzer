#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/PrintReader.h"

class PrintEntities : public Entities {
public:
    PrintEntities();
    ~PrintEntities() override;

    bool isPrint() override;
    EntitiesReader *getReader() override;
    string toString() override;
    int hashCode() override;
};

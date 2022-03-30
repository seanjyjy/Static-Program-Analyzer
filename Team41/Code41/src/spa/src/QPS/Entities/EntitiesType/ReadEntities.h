#pragma once

#include "Entities.h"
#include "QPS/Entities/Readers/ReadReader.h"

class ReadEntities : public Entities {
public:
    ReadEntities();

    ~ReadEntities() override;

    bool isRead() override;

    EntitiesReader *getReader() override;

    string toString() override;

    int hashCode() override;
};

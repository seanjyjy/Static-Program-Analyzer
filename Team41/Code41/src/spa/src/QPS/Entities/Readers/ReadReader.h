#pragma once

#include "QPS/Entities/Readers/EntitiesReader.h"

class ReadReader : public EntitiesReader {
public:
    ~ReadReader() override;

    long long getRowcount(PKBClient* pkb) const override;
    unordered_set<string> getEntities(PKBClient* pkb) override;
};

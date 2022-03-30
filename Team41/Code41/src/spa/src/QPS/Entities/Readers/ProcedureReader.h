#pragma once

#include "EntitiesReader.h"

class ProcedureReader : public EntitiesReader {
public:
    ~ProcedureReader() override;

    long long getRowcount(PKBClient* pkb) const override;
    unordered_set<string> getEntities(PKBClient* pkb) override;
};

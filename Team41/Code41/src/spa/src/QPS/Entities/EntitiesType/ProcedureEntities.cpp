#include "ProcedureEntities.h"

bool ProcedureEntities::isProcedure() {
    return true;
}

string ProcedureEntities::toString() {
    return "procedure";
}

int ProcedureEntities::hashCode() {
    return 5;
}

ProcedureEntities::ProcedureEntities() : Entities(new ProcedureReader()) {}

ProcedureEntities::~ProcedureEntities() {
    delete reader;
}
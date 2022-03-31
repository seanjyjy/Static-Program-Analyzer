#include "StmtEntities.h"

bool StmtEntities::isStmt() {
    return true;
}

string StmtEntities::toString() {
    return "stmt";
}

int StmtEntities::hashCode() {
    return 7;
}

StmtEntities::StmtEntities() : Entities(new StmtReader()) {}

StmtEntities::~StmtEntities() {
    delete reader;
}

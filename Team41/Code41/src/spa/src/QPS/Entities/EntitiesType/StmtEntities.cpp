#include "StmtEntities.h"

bool StmtEntities::isStmt() {
    return true;
}

EntitiesReader* StmtEntities::getReader() {
    return reader;
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

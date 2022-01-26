#include <utility>
#include <vector>

using namespace std;

#include "PKB.h"
#include "StmtTable.h"
#include "TNode.h"

PKB::PKB() {
    stmtTable = new StmtTable();
}

set<string> PKB::getStatements(StmtType type) {
    return stmtTable->getAllStmtsByType(type);
}

StmtType PKB::getStatementType(string stmtNumber) {
    return stmtTable->getStmtType(std::move(stmtNumber));
}

void PKB::setStatement(string stmtNum, StmtType type) {
    return stmtTable->setStmt(std::move(stmtNum), type);
}

int PKB::getStatementCount() {
    return stmtTable->getStmtCount();
}

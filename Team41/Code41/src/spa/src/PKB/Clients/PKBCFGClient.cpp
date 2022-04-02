#include "PKBCFGClient.h"
#include "PKB/Tables/NextTable.h"

#include <utility>

using namespace std;

PKBCFGClient::PKBCFGClient() {
    nextTable = new NextTable();
}

PKBCFGClient::~PKBCFGClient() {
    delete nextTable;
}

//=========================================== CFG =======================================================

CFGNode *PKBCFGClient::getCFGForStmt(const string &stmtNum) const {
    if (stmtToNodeMap.find(stmtNum) == stmtToNodeMap.end()) {
        return nullptr;
    }
    return stmtToNodeMap.at(stmtNum);
}

CFGNode *PKBCFGClient::getRootCFG() const {
    return rootCFG;
}

//=========================================== Next ===================================================

bool PKBCFGClient::isNext(const string &stmt1, const string &stmt2) const {
    return nextTable->isNext(stmt1, stmt2);
}

vector<CFGNode *> PKBCFGClient::getNextNodes(const string &stmt) const {
    return nextTable->getNextNodes(stmt);
}

vector<CFGNode *> PKBCFGClient::getPrevNodes(const string &stmt) const {
    return nextTable->getPrevNodes(stmt);
}

vector<pair<string, string>> PKBCFGClient::getAllNext() const {
    return nextTable->getAllNext();
}

vector<string> PKBCFGClient::getAllStmtsExecBeforeSomeStmt() const {
    return nextTable->getAllStmtsExecBeforeSomeStmt();
}

vector<string> PKBCFGClient::getAllStmtsExecAfterSomeStmt() const {
    return nextTable->getAllStmtsExecAfterSomeStmt();
}

size_t PKBCFGClient::getNumOfStartNodes() const {
    return nextTable->getNumOfStartNodes();
}

size_t PKBCFGClient::getNumOfEndNodes() const {
    return nextTable->getNumOfEndNodes();
}

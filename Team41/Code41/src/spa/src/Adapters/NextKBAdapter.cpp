#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

bool NextKBAdapter::isNext(string stmt1, string stmt2) const {
    return false;
}

string NextKBAdapter::getStmtNext(string stmtNum) const {
    return std::string();
}

string NextKBAdapter::getStmtBefore(string stmtNum) const {
    return std::string();
}

vector<pair<string, string>> NextKBAdapter::getAllNext() const {
    return vector<pair<string, string>>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatHaveNextStmt() const {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatIsNextOfSomeStmt() const {
    return unordered_set<string>();
}

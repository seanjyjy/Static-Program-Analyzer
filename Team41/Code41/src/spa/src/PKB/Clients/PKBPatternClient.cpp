#include "PKBPatternClient.h"

#include <utility>
#include "PKB/Tables/AssignPatternTable.h"
#include "PKB/Tables/IfPatternTable.h"
#include "PKB/Tables/WhilePatternTable.h"

using namespace std;

PKBPatternClient::PKBPatternClient() {
    assignPatternTable = new AssignPatternTable();
    ifPatternTable = new IfPatternTable();
    whilePatternTable = new WhilePatternTable();
}

PKBPatternClient::~PKBPatternClient() {
    delete assignPatternTable;
    delete ifPatternTable;
    delete whilePatternTable;
}

//======================================== Assign ==================================================

unordered_set<string> PKBPatternClient::getAssignStmtFromPattern(TNode *patternAST) const {
    return assignPatternTable->getAllStmtsFromFullPattern(patternAST);
}

unordered_set<string> PKBPatternClient::getAssignStmtFromPatternNVar(TNode *patternAST, string varName) const {
    return assignPatternTable->getStmtFromFullPatternNVar(patternAST, std::move(varName));
}

vector<pair<string, string>> PKBPatternClient::getAssignStmtNVarFromPattern(TNode *patternAST) const {
    return assignPatternTable->getStmtNVarFromFullPattern(patternAST);
}

unordered_set<string> PKBPatternClient::getAssignStmtFromSubpattern(TNode *subPatternAST) const {
    return assignPatternTable->getAllStmtsFromSubPattern(subPatternAST);
}

unordered_set<string> PKBPatternClient::getAssignStmtFromSubpatternNVar(TNode *subPatternAST, const string& varName) const {
    return assignPatternTable->getStmtFromSubPatternNVar(subPatternAST, varName);
}

vector<pair<string, string>> PKBPatternClient::getAssignStmtNVarFromSubpattern(TNode *subPatternAST) const {
    return assignPatternTable->getStmtNVarFromSubPattern(subPatternAST);
}

//======================================== If ==================================================

unordered_set<string> PKBPatternClient::getIfStmtUsingVarCond(string varName) const {
    return ifPatternTable->getStmtFromVar(std::move(varName));
}

vector<pair<string, string>> PKBPatternClient::getIfStmtNVarPairs() const {
    return ifPatternTable->getStmtNVar();
}

unordered_set<string> PKBPatternClient::getIfStmtUsingSomeVarCond() const {
    return ifPatternTable->getStmtsUsingSomeVarInPred();
}

//======================================== While ==================================================

unordered_set<string> PKBPatternClient::getWhileStmtUsingVarCond(string varName) const {
    return whilePatternTable->getStmtFromVar(std::move(varName));
}

vector<pair<string, string>> PKBPatternClient::getWhileStmtNVarPairs() const {
    return whilePatternTable->getStmtNVar();
}

unordered_set<string> PKBPatternClient::getWhileStmtUsingSomeVarCond() const {
    return whilePatternTable->getStmtsUsingSomeVarInPred();
}

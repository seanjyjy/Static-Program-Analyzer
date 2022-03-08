#include <iostream>
#include "PKB/Tables/StmtTable.h"
#include "PKB/Tables/EntityTable.h"
#include "PKB/Tables/UsesTable.h"
#include "PKB/Tables/ModifiesTable.h"
#include "PKB/Tables/FollowsTable.h"
#include "PKB/Tables/ParentTable.h"
#include "PKB/Tables/CallsTable.h"
#include "PKB/Tables/AssignPatternTable.h"
#include "PKB/Tables/WhilePatternTable.h"
#include "PKB/Tables/IfPatternTable.h"

using namespace std;

#include "PKBManager.h"

//======================================== Statements ==================================================

void PKBManager::registerAssignStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, ASSIGN); }

void PKBManager::registerWhileStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, WHILE); }

void PKBManager::registerIfStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, IF); }

void PKBManager::registerReadStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, READ); }

void PKBManager::registerPrintStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, PRINT); }

void PKBManager::registerCallStmt(const string& stmtNum) { return stmtTable->setStmt(stmtNum, CALL); }

//======================================== Entities ==================================================

void PKBManager::registerVariable(const string& varName) { return entityTable->addVariable(varName); }

void PKBManager::registerConstant(const string& constVal) { return entityTable->addConstant(constVal); }

void PKBManager::registerProcedure(const string& procName) { return entityTable->addProcedure(procName); }

//======================================== Calls ==================================================

void PKBManager::registerCalls(const string& proc1, const string& proc2) {
    if (!(isProcedure(proc1))) {
        cout << "Warning: " << "[PKB][registerCalls] Caller is not a registered procedure" << endl;
    }
    if (!(isProcedure(proc2))) {
        cout << "Warning: " << "[PKB][registerCalls] Callee is not a registered procedure" << endl;
    }
    callsTable->setCalls(proc1, proc2);
}

void PKBManager::registerCallsT(const string& proc1, const string& proc2) {
    if (!(isProcedure(proc1))) {
        cout << "Warning: " << "[PKB][registerCallsT] Caller is not a registered procedure" << endl;
    }
    if (!(isProcedure(proc2))) {
        cout << "Warning: " << "[PKB][registerCallsT] Callee is not a registered procedure" << endl;
    }
    callsTable->setCallsT(proc1, proc2);
}

//======================================== Follows ==================================================

void PKBManager::registerFollows(const string& stmt1, const string& stmt2) { return followsTable->setFollows(stmt1, stmt2); }

void PKBManager::registerFollowsT(const string& stmt1, const string& stmt2) { return followsTable->setFollowsT(stmt1, stmt2); }

//======================================== Parent ==================================================

void PKBManager::registerParent(const string& parentStmt, const string& childStmt) {
    return parentTable->setParent(parentStmt, childStmt);
}

void PKBManager::registerParentT(const string& parentStmt, const string& childStmt) {
    return parentTable->setParentT(parentStmt, childStmt);
}

//======================================== Uses ==================================================

void PKBManager::registerUsesS(const string& stmtNum, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isPrintStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum) && !isCallStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerUsesS] Statement is not of type assign/print/if/while/call" << endl;
    }
    return usesTable->setVarUsedInStmt(stmtNum, varName);
}

void PKBManager::registerUsesP(const string& procName, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerUsesP] Procedure is not registered" << endl;
    }
    return usesTable->setVarUsedInProc(procName, varName);
}

//======================================== Modifies ==================================================

void PKBManager::registerModifiesS(const string& stmtNum, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesS] Used variable is not registered" << endl;
    }
    if (!isAssignStmt(stmtNum) && !isReadStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum) && !isCallStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerModifiesS] Statement is not of type assign/read/if/while/call" << endl;
    }
    return modifiesTable->setVarModifiedInStmt(stmtNum, varName);
}

void PKBManager::registerModifiesP(const string& procName, const string& varName) {
    if (!(isVariable(varName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Used variable is not registered" << endl;
    }
    if (!(isProcedure(procName))) {
        cout << "Warning: " << "[PKB][registerModifiesP] Procedure is not registered" << endl;
    }
    return modifiesTable->setVarModifiedInProc(procName, varName);
}

//======================================== Pattern ==================================================

void PKBManager::registerAssignPattern(const string& stmtNum, const string& lhsVariable, TNode *rhsAssignAST) {
    if (!(isAssignStmt(stmtNum))) {
        cout << "Warning: " << "[PKB][registerAssignPattern] Statement is not of type assign" << endl;
    }
    if (!(isVariable(lhsVariable))) {
        cout << "Warning: " << "[PKB][registerAssignPattern] lhs variable is not registered" << endl;
    }
    return assignPatternTable->setPattern(stmtNum, lhsVariable, rhsAssignAST);
}

void PKBManager::registerIfPattern(const string &stmtNum, const string &condVariable) {
    if (!(isAssignStmt(stmtNum))) {
        cout << "Warning: " << "[PKB][registerIfPattern] Statement is not of type if" << endl;
    }
    if (!(isVariable(condVariable))) {
        cout << "Warning: " << "[PKB][registerIfPattern] cond variable is not registered" << endl;
    }
    return ifPatternTable->setPattern(stmtNum, condVariable);
}

void PKBManager::registerWhilePattern(const string &stmtNum, const string &condVariable) {
    if (!(isAssignStmt(stmtNum))) {
        cout << "Warning: " << "[PKB][registerWhilePattern] Statement is not of type while" << endl;
    }
    if (!(isVariable(condVariable))) {
        cout << "Warning: " << "[PKB][registerWhilePattern] cond variable is not registered" << endl;
    }
    return whilePatternTable->setPattern(stmtNum, condVariable);
}

#include <vector>
#include <iostream>
#include "PKB/Tables/StmtTable.h"
#include "PKB/Tables/EntityTable.h"
#include "PKB/Tables/UsesTable.h"
#include "PKB/Tables/ModifiesTable.h"
#include "PKB/Tables/FollowsTable.h"
#include "PKB/Tables/ParentTable.h"
#include "PKB/Tables/PatternTable.h"

using namespace std;

#include "PKBManager.h"

//======================================== Statements ==================================================

void PKBManager::registerAssign(const string& stmtNum) { return stmtTable->setStmt(stmtNum, ASSIGN); }

void PKBManager::registerWhile(const string& stmtNum) { return stmtTable->setStmt(stmtNum, WHILE); }

void PKBManager::registerIf(const string& stmtNum) { return stmtTable->setStmt(stmtNum, IF); }

void PKBManager::registerRead(const string& stmtNum) { return stmtTable->setStmt(stmtNum, READ); }

void PKBManager::registerPrint(const string& stmtNum) { return stmtTable->setStmt(stmtNum, PRINT); }

void PKBManager::registerCall(const string& stmtNum) { return stmtTable->setStmt(stmtNum, CALL); }

//======================================== Entities ==================================================

void PKBManager::registerVariable(const string& varName) { return entityTable->addVariable(varName); }

void PKBManager::registerConstant(const string& constVal) { return entityTable->addConstant(constVal); }

void PKBManager::registerProcedure(const string& procName) { return entityTable->addProcedure(procName); }

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
    if (!isAssignStmt(stmtNum) && !isPrintStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerUsesS] Statement is not of type assign/print/if/while" << endl;
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
    if (!isAssignStmt(stmtNum) && !isReadStmt(stmtNum) && !isIfStmt(stmtNum) && !isWhileStmt(stmtNum)) {
        cout << "Warning: " << "[PKB][registerModifiesS] Statement is not of type assign/read/if/while" << endl;
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

void PKBManager::registerPattern(const string& stmtNum, const string& lhsVariable, TNode *rhsAssignAST) {
    return patternTable->setPattern(stmtNum, lhsVariable, rhsAssignAST);
}

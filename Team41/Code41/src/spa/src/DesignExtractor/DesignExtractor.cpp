#include "DesignExtractor.h"
#include "Common/TNodeType.h"

using namespace std;

DesignExtractor::DesignExtractor(TNode *ast, PKBManager *pkb) : ast(ast), pkb(pkb) {}

void DesignExtractor::extractEntities() {
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    this->nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    this->procSet = ee.getProcSet();

    for (auto [node, stmtNumStr] : nodeToStmtNumMap) {
        TNodeType type = node->getType();
        switch (type) {
            case TNodeType::readStmt:
                pkb->registerReadStmt(stmtNumStr); break;
            case TNodeType::printStmt:
                pkb->registerPrintStmt(stmtNumStr); break;
            case TNodeType::callStmt:
                pkb->registerCallStmt(stmtNumStr); break;
            case TNodeType::whileStmt:
                pkb->registerWhileStmt(stmtNumStr); break;
            case TNodeType::ifStmt:
                pkb->registerIfStmt(stmtNumStr); break;
            case TNodeType::assignStmt:
                pkb->registerAssignStmt(stmtNumStr); break;
        }
    }
    for (const string &procName : ee.getProcSet())
        pkb->registerProcedure(procName);
    for (const string &varName  : ee.getVarSet())
        pkb->registerVariable(varName);
    for (const string &constVal : ee.getConstSet())
        pkb->registerConstant(constVal);
}

void DesignExtractor::extractCalls() {
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extractRelationship();
    this->callsMap = ce.getCallsMap();
    this->procCallOrder = ce.getProcCallOrder();
    for (auto &[procParent, callsSet] : callsMap) {
        for (const string &procChild : callsSet)
            pkb->registerCalls(procParent, procChild);
    }
    for (auto &[procParent, callsTSet] : ce.getCallsTMap()) {
        for (const string &procChild : callsTSet)
            pkb->registerCallsT(procParent, procChild);
    }
}

void DesignExtractor::extractModifies() {
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extractRelationship();
    for (auto &[procName, modifiesSet] : me.getProcModifiesMap()) {
        for (const string &modifiedName : modifiesSet)
            pkb->registerModifiesP(procName, modifiedName);
    }
    for (auto &[stmtNum, modifiesSet] : me.getStmtModifiesMap()) {
        for (const string &modifiedName : modifiesSet)
            pkb->registerModifiesS(stmtNum, modifiedName);
    }
}

void DesignExtractor::extractUses() {
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    ue.extractRelationship();
    for (auto &[procName, usesSet] : ue.getProcUsesMap()) {
        for (const string &usedName : usesSet)
            pkb->registerUsesP(procName, usedName);
    }
    for (auto &[stmtNum, usesSet] : ue.getStmtUsesMap()) {
        for (const string &usedName : usesSet)
            pkb->registerUsesS(stmtNum, usedName);
    }
}

void DesignExtractor::extractFollows() {
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();
    for (auto &[followed, followsTLst] : fe.getFollowsTMap()) {
        pkb->registerFollows(followsTLst.front(), followed); // front of list is direct follows
        for (const string &follower : followsTLst)
            pkb->registerFollowsT(follower, followed);
    }
}

void DesignExtractor::extractParent() {
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();
    for (auto &[parent, parentLst] : pe.getParentMap()) {
        for (const string &child : parentLst)
            pkb->registerParent(parent, child);
    }
    for (auto &[parent, parentTLst] : pe.getParentTMap()) {
        for (const string &child : parentTLst)
            pkb->registerParentT(parent, child);
    }
}

void DesignExtractor::extractPattern() {
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();
    for (auto &[stmt, lhsRhsPair] : pe.getAssignPatternMap()) {
        pkb->registerAssignPattern(stmt, lhsRhsPair.first, lhsRhsPair.second);
    }
}

void DesignExtractor::extractDesign() {
    extractEntities();
    extractCalls();
    extractModifies();
    extractUses();
    extractFollows();
    extractParent();
    extractPattern();
}

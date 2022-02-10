#include "DesignExtractor.h"
#include "Common/TNodeType.h"

using namespace std;

DesignExtractor::DesignExtractor(TNode *ast, PKB *pkb) : ast(ast), pkb(pkb) {}

void DesignExtractor::extractEntities() {
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    this->nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    for (auto [node, stmtNumStr] : nodeToStmtNumMap) {
        TNodeType type = node->getType();
        switch (type) {
            case TNodeType::readStmt:
                pkb->registerRead(stmtNumStr); break;
            case TNodeType::printStmt:
                pkb->registerPrint(stmtNumStr); break;
            case TNodeType::callStmt:
                pkb->registerCall(stmtNumStr); break;
            case TNodeType::whileStmt:
                pkb->registerWhile(stmtNumStr); break;
            case TNodeType::ifStmt:
                pkb->registerIf(stmtNumStr); break;
            case TNodeType::assignStmt:
                pkb->registerAssign(stmtNumStr); break;
        }
    }
    for (string procName : ee.getProcSet())
        pkb->registerProcedure(procName);
    for (string varName  : ee.getVarSet())
        pkb->registerVariable(varName);
    for (string constVal : ee.getConstSet())
        pkb->registerConstant(constVal);
}

void DesignExtractor::extractModifies() {
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();
    for (auto &[procName, modifiesSet] : me.getProcModifiesMap()) {
        for (auto modifiedName : modifiesSet)
            pkb->registerModifiesP(procName, modifiedName);
    }
    for (auto &[stmtNum, modifiesSet] : me.getStmtModifiesMap()) {
        for (auto modifiedName : modifiesSet)
            pkb->registerModifiesS(stmtNum, modifiedName);
    }
}

void DesignExtractor::extractUses() {
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();
    for (auto &[procName, usesSet] : ue.getProcUsesMap()) {
        for (auto usedName : usesSet)
            pkb->registerUsesP(procName, usedName);
    }
    for (auto &[stmtNum, usesSet] : ue.getStmtUsesMap()) {
        for (auto usedName : usesSet)
            pkb->registerUsesS(stmtNum, usedName);
    }
}

void DesignExtractor::extractFollows() {
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();
    for (auto &[followed, followsTLst] : fe.getFollowsTMap()) {
        pkb->registerFollows(followsTLst.front(), followed); // front of list is follows
        for (auto follower : followsTLst)
            pkb->registerFollowsT(follower, followed);
    }
}

void DesignExtractor::extractParent() {
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();
    for (auto &[parent, parentLst] : pe.getParentMap()) {
        for (auto child : parentLst)
            pkb->registerParent(parent, child);
    }
    for (auto &[parent, parentTLst] : pe.getParentTMap()) {
        for (auto child : parentTLst)
            pkb->registerParentT(parent, child);
    }
}

void DesignExtractor::extractPattern() {
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();
    for (auto &[stmt, lhsRhsPair] : pe.getAssignPatternMap()) {
        pkb->registerPattern(stmt, lhsRhsPair.first, lhsRhsPair.second);
    }
}

void DesignExtractor::extractDesign() {
    extractEntities();
    extractModifies();
    extractUses();
    extractFollows();
    extractParent();
    extractPattern();
}

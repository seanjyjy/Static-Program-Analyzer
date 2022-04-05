#include "DesignExtractor.h"
#include "Common/AstNode/TNodeType.h"
#include <Exception/SemanticException.h>

using namespace std;

DesignExtractor::DesignExtractor(TNode *ast, PKBManager *pkb) : ast(ast), pkb(pkb) {}

bool DesignExtractor::extractEntities() {
    EntitiesExtractor ee = EntitiesExtractor(ast);
    if (!ee.extract()) return false; // Entities extraction Failed
    this->nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    this->procSet = ee.getProcSet();

    for (auto[node, stmtNumStr]: nodeToStmtNumMap) {
        TNodeType type = node->getType();
        switch (type) {
            case TNodeType::readStmt:
                pkb->registerReadStmt(stmtNumStr, node->getChildren()[0]->getTokenVal());
                break;
            case TNodeType::printStmt:
                pkb->registerPrintStmt(stmtNumStr, node->getChildren()[0]->getTokenVal());
                break;
            case TNodeType::callStmt:
                pkb->registerCallStmt(stmtNumStr, node->getChildren()[0]->getTokenVal());
                break;
            case TNodeType::whileStmt:
                pkb->registerWhileStmt(stmtNumStr);
                break;
            case TNodeType::ifStmt:
                pkb->registerIfStmt(stmtNumStr);
                break;
            case TNodeType::assignStmt:
                pkb->registerAssignStmt(stmtNumStr);
                break;
            default:
                break;
        }
    }
    for (const string &procName: ee.getProcSet())
        pkb->registerProcedure(procName);
    for (const string &varName: ee.getVarSet())
        pkb->registerVariable(varName);
    for (const string &constVal: ee.getConstSet())
        pkb->registerConstant(constVal);
    return true;
}

bool DesignExtractor::extractCalls() {
    CallsExtractor ce = CallsExtractor(ast, procSet);
    if (!ce.extract()) return false; // Calls extraction failed

    this->callsMap = ce.getCallsMap();
    this->procCallOrder = ce.getProcCallOrder();
    for (auto &[procParent, callsSet]: callsMap) {
        for (const string &procChild: callsSet)
            pkb->registerCalls(procParent, procChild);
    }
    for (auto &[procParent, callsTSet]: ce.getCallsTMap()) {
        for (const string &procChild: callsTSet)
            pkb->registerCallsT(procParent, procChild);
    }
    return true;
}

bool DesignExtractor::extractModifies() {
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    for (auto &[procName, modifiesSet]: me.getProcModifiesMap()) {
        for (const string &modifiedName: modifiesSet)
            pkb->registerModifiesP(procName, modifiedName);
    }
    for (auto &[stmtNum, modifiesSet]: me.getStmtModifiesMap()) {
        for (const string &modifiedName: modifiesSet)
            pkb->registerModifiesS(stmtNum, modifiedName);
    }
    return true;
}

bool DesignExtractor::extractUses() {
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    ue.extract();

    for (auto &[procName, usesSet]: ue.getProcUsesMap()) {
        for (const string &usedName: usesSet)
            pkb->registerUsesP(procName, usedName);
    }
    for (auto &[stmtNum, usesSet]: ue.getStmtUsesMap()) {
        for (const string &usedName: usesSet)
            pkb->registerUsesS(stmtNum, usedName);
    }
    return true;
}

bool DesignExtractor::extractFollows() {
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    for (auto &[followed, followsTLst]: fe.getFollowsTMap()) {
        pkb->registerFollows(followed, followsTLst.front()); // front of list is direct follows
        for (const string &follower: followsTLst)
            pkb->registerFollowsT(followed, follower);
    }
    return true;
}

bool DesignExtractor::extractParent() {
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    for (auto &[parent, parentLst]: pe.getParentMap()) {
        for (const string &child: parentLst)
            pkb->registerParent(parent, child);
    }
    for (auto &[parent, parentTLst]: pe.getParentTMap()) {
        for (const string &child: parentTLst)
            pkb->registerParentT(parent, child);
    }
    return true;
}

bool DesignExtractor::extractPattern() {
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    for (auto &[stmt, lhsRhsPair]: pe.getAssignPatternMap()) {
        pkb->registerAssignPattern(stmt, lhsRhsPair.first, lhsRhsPair.second);
    }
    for (auto &[stmt, varSet]: pe.getIfPatternMap()) {
        for (const string &var: varSet)
            pkb->registerIfPattern(stmt, var);
    }
    for (auto &[stmt, varSet]: pe.getWhilePatternMap()) {
        for (const string &var: varSet)
            pkb->registerWhilePattern(stmt, var);
    }
    return true;
}

bool DesignExtractor::extractCFG() {
    CFGExtractor ce = CFGExtractor(ast, nodeToStmtNumMap);
    ce.extract();
    pkb->registerCFG(ce.getCFG(), ce.getStmtNumToNodeMap());
    return true;
}

bool DesignExtractor::extractDesign() {
    if (!ast) return false; // ast is nullptr
    bool isSuccess = extractEntities()
                     && extractFollows()
                     && extractParent()
                     && extractCalls()
                     && extractModifies()
                     && extractUses()
                     && extractPattern()
                     && extractCFG();
    return isSuccess;
}

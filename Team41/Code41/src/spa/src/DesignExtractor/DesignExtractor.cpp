#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#include "DesignExtractor.h"
#include "Common/TNodeType.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"

DesignExtractor::DesignExtractor(TNode *ast, PKB *pkb) : ast(ast), pkb(pkb) {}

void DesignExtractor::registerEntity(TNode *node, int &stmtNum) {
    TNodeType type = node->getType();
    if (isStatement(type)) {
        stmtNum += 1;
        string stmtNumStr = to_string(stmtNum);
        nodeToStmtNumMap.insert({node, stmtNumStr});
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
    } else {
        //string val = node->getVal()->getVal();
        switch (type) {
            case TNodeType::varName:
                pkb->registerVariable(node->getVal()->getVal()); break;
            case TNodeType::constValue:
                pkb->registerConstant(node->getVal()->getVal()); break;
        }
    }
}

void DesignExtractor::registerEntities() { // todo register P calls P
    int stmtNum = 0;
    stack<TNode *> stk;
    stk.push(ast);
    while (!stk.empty()) {
        TNode *node = stk.top(); stk.pop();
        registerEntity(node, stmtNum);
        vector<TNode *> ch = node->getChildren();
        reverse(ch.begin(), ch.end()); // left to right dfs
        for (TNode *child : ch) {
            stk.push(child);
        }
    }
}

void DesignExtractor::registerProcedures() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        pkb->registerProcedure(procNode->getVal()->getVal());
    }
}

void DesignExtractor::extractDesign() {
    registerProcedures();
    registerEntities();
    ModifiesExtractor me = ModifiesExtractor{ast, pkb, nodeToStmtNumMap};
    me.extractRelationship();
    UsesExtractor ue = UsesExtractor{ast, pkb, nodeToStmtNumMap};
    ue.extractRelationship();
}

void DesignExtractor::printStmtNums() {
    for (auto [node, stmtNumStr] : nodeToStmtNumMap)
        std::cout << stmtNumStr << ": " << TNode::typeToString(node->getType()) << endl;
}

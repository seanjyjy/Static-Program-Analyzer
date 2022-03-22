#include <queue>
#include <iostream>

#include "CFGExtractor.h"
#include "Common/TNodeType.h"

CFGExtractor::CFGExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        StmtNumExtractor(ast, nodeToStmtNumMap) {}

CFGNode *CFGExtractor::createCFGNode(TNode *tNode) {
    CFGNode *cfgNode = new CFGNode(nodeToStmtNumMap.at(tNode));
    stmtNumToNodeMap.insert({nodeToStmtNumMap.at(tNode), cfgNode}); // set stmtNum to cfgNode
    return cfgNode;
}

void CFGExtractor::addCFGEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode) {
    parentCFGNode->addChild(childCFGNode);
    if (parentCFGNode->getStmtNum() != ROOT_INDEX) // do not add parent edge if parent is root
        childCFGNode->addParent(parentCFGNode);
}

void CFGExtractor::dfsInitCFG(TNode *curTNode, CFGNode *curCFGNode, CFGNode *parentCFGNode) {
    TNodeType type = curTNode->getType();
    if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = curTNode->getChildren();
        CFGNode *childCFGNode = createCFGNode(ch[0]);
        if (parentCFGNode) // IF or WHILE CFGNode point to first stmt in container
            addCFGEdge(parentCFGNode, childCFGNode); // add forward CFG edge

        for (size_t i = 0; i < ch.size(); ++i) {
            TNodeType childType = ch[i]->getType();
            // TODO ch.size() - 1 may cause unsigned integer overflow, dangerous?
            CFGNode *neighbourCFGNode = (i != ch.size() - 1) ? createCFGNode(ch[i + 1])
                                                             : nullptr; // last child has no neighbour
            if (childType == TNodeType::ifStmt) {
                dfsInitCFG(ch[i], childCFGNode, nullptr);
            } else {
                if (neighbourCFGNode)
                    addCFGEdge(childCFGNode, neighbourCFGNode);
                if (childType == TNodeType::whileStmt)
                    dfsInitCFG(ch[i], childCFGNode, nullptr);
            }
            childCFGNode = neighbourCFGNode;
        }
    } else if (type == TNodeType::ifStmt) { // bfs down with {stmtLst, nullptr, curCFGNode}
        vector<TNode *> ch = curTNode->getChildren();
        dfsInitCFG(ch[1], nullptr, curCFGNode); // stmtLst has no CFGNode
        dfsInitCFG(ch[2], nullptr, curCFGNode);
    } else if (type == TNodeType::whileStmt) { // bfs down with {stmtLst, nullptr, curCFGNode}
        dfsInitCFG(curTNode->getChildren()[1], nullptr, curCFGNode);
    }
}

void CFGExtractor::addBackEdge(TNode *fromTNode, TNode *toTNode) {
    const string &fromStmtNum = nodeToStmtNumMap.at(fromTNode), &toStmtNum = nodeToStmtNumMap.at(toTNode);
    CFGNode *fromCFGNode = stmtNumToNodeMap.at(fromStmtNum), *toCfgNode = stmtNumToNodeMap.at(toStmtNum);
    addCFGEdge(fromCFGNode, toCfgNode);
}

void CFGExtractor::dfsLinkBack(TNode *curTNode, TNode *backTNode) {
    TNodeType type = curTNode->getType();
    if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = curTNode->getChildren();
        int len = (int) ch.size();
        for (int i = 0; i < len - 1; ++i) {
            TNodeType childType = ch[i]->getType();
            if (childType == TNodeType::ifStmt)
                dfsLinkBack(ch[i], ch[i + 1]); // end of IF will link to IF's neighbour node
            else if (childType == TNodeType::whileStmt)
                dfsLinkBack(ch[i], ch[i]); // end of WHILE will link back to WHILE
        }

        TNode *lastChild = ch.back();
        TNodeType lastType = lastChild->getType();
        if (lastType == TNodeType::ifStmt) {
            if (backTNode)
                dfsLinkBack(lastChild, backTNode); // end of this IF will link to backTNode
            else
                dfsLinkBack(lastChild, nullptr); // end of this IF will link to nothing

        } else {
            if (backTNode)
                addBackEdge(lastChild, backTNode); // other stmts link to backTNode
            if (lastType == TNodeType::whileStmt)
                dfsLinkBack(lastChild, lastChild); // end of WHILE will link back to WHILE
        }
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = curTNode->getChildren();
        dfsLinkBack(ch[1], backTNode); // pass to stmtLst to handle
        dfsLinkBack(ch[2], backTNode);
    } else if (type == TNodeType::whileStmt) {
        dfsLinkBack(curTNode->getChildren()[1], backTNode); // pass to stmtLst to handle
    }
}

void CFGExtractor::extract() {
    stmtNumToNodeMap.insert({ROOT_INDEX, cfg});
    vector<TNode *> procTNodes = ast->getChildren();
    for (TNode *procNode: procTNodes) { // start dfs from stmtLst of each procedure
        dfsInitCFG(procNode->getChildren()[0], nullptr, cfg); // {stmtLst, nullptr, root CFG}
    }
    for (TNode *procNode: procTNodes) {
        dfsLinkBack(procNode->getChildren()[0], nullptr); // {stmtLst, nullptr, root CFG}
    }
}

CFGNode *CFGExtractor::getCFG() {
    return this->cfg;
}

unordered_map<string, CFGNode *> CFGExtractor::getStmtNumToNodeMap() {
    return this->stmtNumToNodeMap;
}

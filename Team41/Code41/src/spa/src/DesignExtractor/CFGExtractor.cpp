#include <queue>

#include "CFGExtractor.h"
#include "Common/TNodeType.h"

CFGExtractor::CFGExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

CFGNode *CFGExtractor::createCFGNode(TNode *tNode) {
    CFGNode *cfgNode = new CFGNode(nodeToStmtNumMap[tNode]);
    stmtNumToNodeMap[nodeToStmtNumMap[tNode]] = cfgNode; // set stmtNum to cfgNode
    return cfgNode;
}

void CFGExtractor::addCFGEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode, bool isForward) {
    if (isForward)
        parentCFGNode->addForwardChild(childCFGNode);
    else
        parentCFGNode->addBackwardChild(childCFGNode);
    parentCFGNode->addParent(childCFGNode);
}

void CFGExtractor::buildInitCFG() {
    queue<tuple<TNode *, CFGNode *, CFGNode *>> bfsQ; // {curTNode, curCFGNode, parentCFGNode}
    for (TNode *procNode : ast->getChildren()) { // start bfs from stmtLst of each procedure
        bfsQ.push({procNode->getChildren()[0], nullptr, cfg}); // {stmtLst, nullptr, root CFG}
    }

    while (!bfsQ.empty()) {
        auto [curTNode, curCFGNode, parentCFGNode] = bfsQ.front();
        bfsQ.pop();

        TNodeType type = curTNode->getType();
        if (type == TNodeType::stmtLst) {
            vector<TNode *> ch = curTNode->getChildren();
            CFGNode *childCFGNode = createCFGNode(ch[0]);
            if (parentCFGNode) // IF or WHILE CFGNode point to first stmt in container
                addCFGEdge(parentCFGNode, childCFGNode, true); // add forward CFG edge

            for (int i = 0; i < ch.size(); ++i) {
                TNodeType childType = ch[i]->getType();
                CFGNode *neighbourCFGNode = (i != ch.size() - 1) ? createCFGNode(ch[i+1])
                                                                 : nullptr; // last child has no neighbour
                if (childType == TNodeType::ifStmt) {
                    bfsQ.push({ch[i], childCFGNode, nullptr});
                } else {
                    if (neighbourCFGNode)
                        addCFGEdge(childCFGNode, neighbourCFGNode, true);
                    if (childType == TNodeType::whileStmt)
                        bfsQ.push({ch[i], childCFGNode, nullptr});
                }
                childCFGNode = neighbourCFGNode;
            }
        } else if (type == TNodeType::ifStmt) { // bfs down with {stmtLst, nullptr, curCFGNode}
            vector<TNode *> ch = curTNode->getChildren();
            bfsQ.push({ch[1], nullptr, curCFGNode}); // stmtLst has no CFGNode
            bfsQ.push({ch[2], nullptr, curCFGNode});
        } else if (type == TNodeType::whileStmt) { // bfs down with {stmtLst, nullptr, curCFGNode}
            bfsQ.push({curTNode->getChildren()[1], nullptr, curCFGNode});
        }
    }
}

void CFGExtractor::addBackEdge(TNode *fromTNode, TNode *toTNode) {
    const string &fromStmtNum = nodeToStmtNumMap[fromTNode], &toStmtNum = nodeToStmtNumMap[toTNode];
    CFGNode *fromCFGNode = stmtNumToNodeMap[fromStmtNum], *toCfgNode = stmtNumToNodeMap[toStmtNum];
    addCFGEdge(fromCFGNode, toCfgNode, false); // backwards edge thus false parameter
}

void CFGExtractor::linkBackNode() {
    queue<pair<TNode *, TNode *>> bfsQ; // {curTNode, backTNode}
    for (TNode *procNode : ast->getChildren()) {
        bfsQ.push({procNode->getChildren()[0], nullptr}); // {stmtLst, nullptr, root CFG}
    }

    while (!bfsQ.empty()) {
        auto [curTNode, backTNode] = bfsQ.front();
        bfsQ.pop();

        TNodeType type = curTNode->getType();
        if (type == TNodeType::stmtLst) {
            vector<TNode *> ch = curTNode->getChildren();
            for (int i = 0; i < ch.size() - 1; ++i) {
                TNodeType childType = ch[i]->getType();
                if (childType == TNodeType::ifStmt)
                    bfsQ.push({ch[i], ch[i+1]}); // end of IF will link to IF's neighbour node
                else if (childType == TNodeType::whileStmt)
                    bfsQ.push({ch[i], ch[i]}); // end of WHILE will link back to WHILE
            }

            TNode* lastChild = ch.back();
            TNodeType lastType = lastChild->getType();
            if (lastType == TNodeType::ifStmt) {
                if (backTNode)
                    bfsQ.push({lastChild, backTNode}); // end of this IF will link to backTNode
            } else {
                if (backTNode)
                    addBackEdge(lastChild, backTNode); // other stmts link to backTNode
                if (lastType == TNodeType::whileStmt)
                    bfsQ.push({lastChild, lastChild}); // end of WHILE will link back to WHILE
            }
        } else if (type == TNodeType::ifStmt) {
            vector<TNode *> ch = curTNode->getChildren();
            bfsQ.push({ch[1], backTNode}); // pass to stmtLst to handle
            bfsQ.push({ch[2], backTNode});
        } else if (type == TNodeType::whileStmt) {
            bfsQ.push({curTNode->getChildren()[1], backTNode}); // pass to stmtLst to handle
        }
    }
}

void CFGExtractor::extractCFG() {
    buildInitCFG();
    linkBackNode();
}

CFGNode *CFGExtractor::getCFG() {
    return this->cfg;
}

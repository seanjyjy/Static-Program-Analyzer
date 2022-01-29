#include "TNode.h"
#include "SimpleParser/Token.h"

TNode::TNode(TNodeType type, Token val): type(type), val(move(val)) {}

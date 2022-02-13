#include "PatternClause.h"

PatternClause::PatternClause(QueryDeclaration synonym, ClauseVariable lhs, TNode *miniAST) :
    synonym(synonym), lhs(lhs), miniAST(miniAST) {}

QueryDeclaration PatternClause::getSynonym() const { return synonym; }
ClauseVariable PatternClause::getLHS() const { return lhs; }
TNode *PatternClause::getRHS() const { return miniAST; }
void PatternClause::setIsWildcard() { type = wildcard; }
void PatternClause::setIsFullPattern() { type = fullpattern; }
void PatternClause::setIsSubPattern() { type = subpattern; }
bool PatternClause::isWildcard() { return type == wildcard; }
bool PatternClause::isFullPattern() { return type == fullpattern; }
bool PatternClause::isSubPattern() { return type == subpattern; }

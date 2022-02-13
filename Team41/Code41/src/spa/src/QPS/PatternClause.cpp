#include "PatternClause.h"

PatternClause::PatternClause(clause_type type, ClauseVariable left, ClauseVariable right, QueryDeclaration synonym) :
 QueryClause(type, left, right), synonym(synonym) {
    
}
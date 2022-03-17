#include "WithClause.h"

WithClause::WithClause(WithVariable left, WithVariable right):left(left), right(right){}

WithVariable WithClause::getLeft() {
    return left;
}

WithVariable WithClause::getRight() {
    return right;
}

#include "Evaluator.h"

Table* Evaluator::evaluate(string clause, PKB *pkb) {
//    PQLTable* table = new PQLTable();
    Table* table = nullptr;
    return table;
    // Follows : 'Follows' '(' stmtRef ',' stmtRef ')'
    // stmtRef: synonym | '_' | INTEGER
    // so this is 9 combinations?

    // INTEGER - INTEGER -> true or false
    // INTEGER - SYNONYM -> true or false or table?
    // INTEGER - '_' -> true or false

    // SYNONYM - INTEGER -> true or false or table?
    // SYNONYM - SYNONYM -> a table
    // SYNONYM - '_' -> a table

    // '_' - INTEGER -> true or false
    // '_' - SYNONYM -> a table
    // '_' - '_' -> true or false (Which is just checking there is > 1 statements?)
}
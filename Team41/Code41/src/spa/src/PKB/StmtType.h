//
// Created by JinHao on 27/1/22.
//

#ifndef SPA_STMTTYPE_H
#define SPA_STMTTYPE_H

/**
 * The types of SIMPLE statements
 */
enum StmtType {
    UNKNOWN_STMT,
    ASSIGN,
    WHILE,
    IF,
    READ,
    PRINT,
    CALL
};

#endif //SPA_STMTTYPE_H

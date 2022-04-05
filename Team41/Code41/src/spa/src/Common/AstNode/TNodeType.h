#pragma once

// Represents the type of a SIMPLE Abstract Syntax Tree node.
enum class TNodeType {
    program,
    procedure,
    stmtLst,
    readStmt, // read x
    printStmt, // print x
    callStmt, // call "main"
    whileStmt, // while
    ifStmt, // if then else
    assignStmt, // =
    notOp, // !
    andOp, // &&
    orOp, // ||
    gt, // >
    ge, // >=
    lt, // <
    le, // <=
    eq, // ==
    ne, // !=
    plus, // +
    minus, // -
    times, // *
    div, // /
    mod, // %
    varName, // x
    procName, // "main"
    constValue, // 123

    dummy, // for parsing to ast
};


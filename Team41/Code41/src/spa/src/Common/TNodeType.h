#pragma once

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

static bool isStatement(TNodeType type) {
    switch (type) {
        case TNodeType::readStmt:
            return true;
        case TNodeType::printStmt:
            return true;
        case TNodeType::callStmt:
            return true;
        case TNodeType::whileStmt:
            return true;
        case TNodeType::ifStmt:
            return true;
        case TNodeType::assignStmt:
            return true;
        default:
            return false;
    }
}

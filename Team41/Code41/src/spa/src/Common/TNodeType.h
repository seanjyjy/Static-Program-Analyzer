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

static bool isCondExpr(TNodeType type) {
    switch (type) {
        case TNodeType::notOp:
            return true;
        case TNodeType::andOp:
            return true;
        case TNodeType::orOp:
            return true;
        case TNodeType::gt:
            return true;
        case TNodeType::ge:
            return true;
        case TNodeType::lt:
            return true;
        case TNodeType::le:
            return true;
        case TNodeType::eq:
            return true;
        case TNodeType::ne:
            return true;
        default:
            return false;
    }
}

static bool isOp(TNodeType type) {
    switch (type) {
        case TNodeType::plus:
            return true;
        case TNodeType::minus:
            return true;
        case TNodeType::times:
            return true;
        case TNodeType::div:
            return true;
        case TNodeType::mod:
            return true;
        default:
            return false;
    }
}

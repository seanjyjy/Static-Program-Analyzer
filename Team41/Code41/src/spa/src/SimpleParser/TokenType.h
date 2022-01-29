#pragma once

enum class TokenType {
    openingBrace, // {
    closingBrace, // }
    openingBracket, // (
    closingBracket, // )
    assign, // =
    semicolon, // ;
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
    keyword, // procedure, read, printRecursive, call, while, if, then, else
    name, // LETTER (LETTER|DIGIT)*
    integer, // DIGIT+
    eof, // special token indicating end of file
};


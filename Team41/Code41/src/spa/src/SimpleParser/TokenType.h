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
    name, // LETTER (LETTER|DIGIT)*
    integer, // DIGIT+
    eof, // special token indicating end of file
};


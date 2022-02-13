#pragma once

#include "Tokens.h"
#include "Common/TNode.h"

using namespace std;

class Parser {
private:
    // affects which grammar rule is checked when parsing
    enum class Option {
        program,
        procedure,
        stmt,
        readStmt,
        printStmt,
        callStmt,
        whileStmt,
        ifStmt,
        assignStmt,
        condExpr,
        expr,
        term,
        name,
        constant,
    };

    string input; // input string to tokenize
    int cursor; // keep track of position in input
    Tokens tokens; // tokens after tokenizing
    Token currToken; // current token being parsed
    Option parseOption = Option::program; // decides how input is parsed

    /**
     * Copies the current index of the cursor and returns it.
     *
     * @return the current index of the input string currently being parsed.
     */
    int saveCursor();

    /**
     * Moves the current index of the cursor to the given position.
     *
     * @param to an integer index, between 0 and the size of input.
     */
    void backtrack(int to);

    /**
     * Moves the cursor forward by one and updates the current token.
     */
    void advance();

    /**
     * Checks that the current token matches the given type, throwing if it does not.
     *
     * @param type the token type to match
     */
    void expect(TokenType type);

    /**
     * Checks that the current token matches the given type and value, throwing if it does not.
     *
     * @param type the token type to match
     * @param val the token value to match
     */
    void expect(TokenType type, string val);

    /**
     * Verifies the current token matches the type, advances the cursor and returns the matched token.
     * Throws if the match fails. Mainly a convenience function.
     *
     * @param type the token type to match
     * @return the token matched before the cursor is advanced.
     */
    Token *checkAndAdvance(TokenType type);

    /**
     * Verifies the current token matches the type and value, advances the cursor and returns the matched token.
     * Throws if the match fails. Mainly a convenience function.
     *
     * @param type the token type to match
     * @param val the token value to match
     * @return the token matched before the cursor is advanced.
     */
    Token *checkAndAdvance(TokenType type, string val);

    /**
     * Checks if the current token matches the given type, but does not throw.
     *
     * @param type the token type to match
     * @return true if the current token matches the given type, false otherwise.
     */
    bool peekMatchType(TokenType type);

    /**
     * Checks if the current token matches the given type and value, but does not throw.
     *
     * @param type token type to match
     * @param val token value to match
     * @return true if the current token matches the given type, false otherwise.
     */
    bool peekMatchTypeVal(TokenType type, string val);

    /**
     * Checks if all tokens have been consumed.
     *
     * @return true if the current token is the special EOF token, false otherwise.
     */
    bool isEof();

    /**
     * Adds a suffix to the given string indicating the row/col the current token started/ended parsing.
     *
     * @param s the string to augment.
     * @return the augmented string.
     */
    string withCurrToken(const string &s);

    /**
     * Generates an error message including start and end parse positions of the current token.
     *
     * @return a string describing the error.
     */
    string genericErrorMsg();

    /**
     * Generates a syntax error message including start and end parse positions of the current token.
     *
     * @return a string describing the error.
     */
    string syntaxErrorMsg();

    /**
     * Parses a program. program -> procedure+
     *
     * @return a program node with children procedure nodes.
     */
    TNode *eatProgram();

    /**
     * Parses a procedure. procedure -> 'procedure' proc_name '{' stmtLst '}'
     *
     * @return a procedure node with a child stmtLst node.
     */
    TNode *eatProcedure();

    /**
     * Parses a statement list. stmtLst -> stmt+
     *
     * @return a stmtLst node with children read/print/call/while/if/assign nodes.
     */
    TNode *eatStmtLst();

    /**
     * Parses a statement. stmt -> read | print | call | while | if | assign
     *
     * @return the read/print/call/while/if/assign node.
     */
    TNode *eatStmt();

    /**
     * Parses a read statement. read -> 'read' var_name ';'
     *
     * @return the read node with a child var node.
     */
    TNode *eatStmtRead();

    /**
     * Parses a print statement. print -> 'print' var_name ';'
     *
     * @return the print node with a child var node.
     */
    TNode *eatStmtPrint();

    /**
     * Parses a call statement. call -> 'call' proc_name ';'
     *
     * @return the call node with a child var node.
     */
    TNode *eatStmtCall();

    /**
     * Parses a while statement. while -> 'while' '(' cond_expr ')' '{' stmtLst '}'
     *
     * @return the while node with children condexpr and stmtlst nodes.
     */
    TNode *eatStmtWhile();

    /**
     * Parses an if statement. if -> 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
     *
     * @return the if node with children condexpr, stmtlst and stmtlst nodes.
     */
    TNode *eatStmtIf();

    /**
     * Parses an assign statement. assign -> var_name '=' expr ';'
     *
     * @return the assign node with children var and expr nodes.
     */
    TNode *eatStmtAssign();

    /**
     * Parses a conditional expression.
     * cond_expr  -> rel_expr
     *          | '!' '(' cond_expr ')'
     *          | '(' cond_expr ')' '&&' '(' cond_expr ')'
     *          | '(' cond_expr ')' '||' '(' cond_expr ')'
     *
     * @return the plus | minus | times | div | mod | not | and | or node with their respective children.
     */
    TNode *eatCondExpr();

    /**
     * Parses a relational expression.
     * rel_expr   -> rel_factor '>' rel_factor
     *              | rel_factor '>=' rel_factor
     *              | rel_factor '<' rel_factor
     *              | rel_factor '<=' rel_factor
     *              | rel_factor '==' rel_factor
     *              | rel_factor '!=' rel_facto
     *
     * @return the > | >= | < | <= | == | != node with their respective left and right child.
     */
    TNode *eatRelExpr();

    /**
     * Parses a relational factor. rel_factor -> var_name | const_value | expr
     *
     * @return the var | const | expr nodes with their respective children.
     */
    TNode *eatRelFactor();

    /**
     * Parses an expression. expr -> term expr1 | term
     *
     * @return the + | - node with their respective left and right children.
     */
    TNode *eatExpr();

    /**
     * Parses an intermediate expression. expr1 -> '+' term expr1 | '-' term expr1 | '+' term | '-' term
     *
     * @return the intermediate + | - node with their respective left and right children.
     */
    TNode *eatExpr1();

    /**
     * Parses a term. term -> factor term1 | factor
     *
     * @return the * | / | % node with their respective left and right children.
     */
    TNode *eatTerm();

    /**
     * Parses a intermediate term. term -> factor term1 | factor
     *
     * @return the intermediate * | / | % node with their respective left and right children.
     */
    TNode *eatTerm1();

    /**
     * Parses a factor. factor -> var_name | const_value | '(' expr ')'
     *
     * @return the var | const | expr node with their respective children.
     */
    TNode *eatFactor();

    /**
     * Sets up parse state. Must be called before parse for correct behaviour.
     *
     * @param s the input string to parse.
     */
    void init(const string &s);

    /**
     * Parses the string into an AST.
     *
     * @param s the input string to parse.
     * @return the parsed AST node.
     */
    TNode *parse(const string &s);

public:
    Parser();

    /**
     * Parses a program string.
     *
     * @param s the program string.
     * @return the program AST node.
     */
    TNode *parseProgram(const string &s);

    /**
     * Parses an expression string into an abstract syntax tree (AST).
     * Used by the query evaluator for expression-spec.
     * expr -> expr + term | expr - term | term
     *
     * @param s the expr string with optional leading/trailing whitespace
     * @return the parsed AST
     */
    TNode *parseExpr(const string &s);

    /**
     * Parses a procedure string. Only used in tests.
     *
     * @param s the procedure string
     * @return the procedure AST node.
     */
    TNode *parseProcedure(const string &s);

    /**
     * Parses a statement. Only used in tests.
     *
     * @param s the statement string
     * @return the read/print/while/if/assign AST node.
     */
    TNode *parseStmt(const string &s);

    /**
     * Parses a read statement. Only used in tests.
     *
     * @param s the read statement string
     * @return the read AST node.
     */
    TNode *parseRead(const string &s);

    /**
     * Parses a print statement. Only used in tests.
     *
     * @param s the print statement string
     * @return the print AST node
     */
    TNode *parsePrint(const string &s);

    /**
     * Parses a call statement. Only used in tests.
     *
     * @param s the call statement string
     * @return the call AST node
     */
    TNode *parseCall(const string &s);

    /**
     * Parses a while statement. Only used in tests.
     *
     * @param s the while statement string
     * @return the while AST node
     */
    TNode *parseWhile(const string &s);

    /**
     * Parses an if statement. Only used in tests.
     *
     * @param s the if statement string
     * @return the if AST node
     */
    TNode *parseIf(const string &s);

    /**
     * Parses an assign statement. Only used in tests.
     *
     * @param s the assign statement string
     * @return the assign AST node
     */
    TNode *parseAssign(const string &s);

    /**
     * Parses a conditional expression. Only used in tests.
     *
     * @param s the conditional expression statement string
     * @return the plus/minus/times/div/mod/and/or AST node
     */
    TNode *parseCondExpr(const string &s);

    /**
     * Parses a term. Only used in tests.
     *
     * @param s the term statement string.
     * @return the var/const/plus/minus/times/div/mod/and/or AST node
     */
    TNode *parseTerm(const string &s);

    /**
     * Parses a name. Only used in tests.
     *
     * @param s the name to parse
     * @return the var AST node
     */
    TNode *parseName(const string &s);

    /**
     * Parses a const. Only used in tests.
     *
     * @param s the const to parse
     * @return the const AST node
     */
    TNode *parseConst(const string &s);

    /**
     * Prints all tokens. For debugging purposes.
     */
    void printTokens();
};
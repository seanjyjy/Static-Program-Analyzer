#include <stdio.h>

#include "QueryParser.h"

using namespace std;

bool QueryParser::skipSuchThat() {
    optional<string> such = lex->nextExpected("such");
    if (!such.has_value()) {
        printf("Malformed input. Expected keyword: such\n");
        return false;
    }
    optional<string> that = lex->nextExpected("that");
    if (!that.has_value()) {
        printf("Malformed input. Expected keyword: that\n");
        return false;
    }
    return true;
}

bool QueryParser::parseDeclarations() {
    do {
        if (lex->isEndOfQuery()) {
            printf("Query Incomplete.");
            return false;
        }
        optional<string> type = lex->nextDeclarationType();
        if (type == nullopt) {
            printf("Syntax Error: Invalid declaration type <%s>\n", lex->nextToken().c_str());
            return false;
        }
        if (!parseDeclarationsOfTypeString(type->c_str())){
            return false;
        }
    } while (!lex->peekNextIsString("Select"));
    return true;
}

bool QueryParser::parseDeclarationsOfTypeString(string type) {
    // for same type, same line (TYPE syn1, syn2,...;)
    bool isSetEnd = false;
    // Check if is same type, same line (TYPE syn1, syn2,...;)
    while (!isSetEnd) {
        // Read a synonym
        optional<string> synonym = lex->nextSynonym();
        if (synonym == nullopt) {
            printf("Syntax Error: Invalid synonym <%s>\n", lex->nextToken().c_str());
            return false;
        }
        // Generate the QueryDeclaration object
        generateDeclarationObject(type, synonym->c_str());
        // Check if there's a comma to carry on same design-entity declaration
        if (lookForDeclarationComma()) continue;
        // No comma? Expect a ";" for end of a declaration type
        if (lookForDeclarationSemiColon()) {
            isSetEnd = true;
        } else {
            return false;
        }
    }
    return true;
}

void QueryParser::generateDeclarationObject(string type, string synonym) {
    QueryDeclaration::design_entity_type t = QueryDeclaration::stringToType(type);
    QueryDeclaration qd(t, synonym);
    queryObject->declarations.push_back(qd);
}

bool QueryParser::lookForDeclarationSemiColon() {
    if (lex->peekNextIsString(";")) {
        lex->nextExpected(";");
        return true;
    } else {
        printf("Syntax error: Expected a ';' for end of declaration.\n");
        return false;
    }
}

bool QueryParser::lookForDeclarationComma() {
    if (lex->peekNextIsString(",")) {
        lex->nextExpected(",");
        return true;
    }
    return false;
}

optional<QueryDeclaration> QueryParser::findMatchingDeclaration(string synonym) {
    for (QueryDeclaration qd : queryObject->declarations) {
        if (qd.synonym == synonym)
            return qd;
    }
    return nullopt;
}

QueryDeclaration::design_entity_type QueryParser::determineDeclarationType(string synonym) {
    optional<QueryDeclaration> qd = findMatchingDeclaration(synonym);
    if (qd == nullopt) {
        // not a declared synonym
        return QueryDeclaration::NONE;
    } else {
        return qd->type;
    }
}

bool QueryParser::parseSelectSynonym() {
    lex->nextExpected("Select");
    optional<string> synonym = lex->nextSynonym();
    if (!synonym.has_value()) {
        printf("Syntax Error: Use of select with no synonym.\n");
        return false;
    }
    optional<QueryDeclaration> qd = findMatchingDeclaration(*synonym);
    if (!qd.has_value()) {
        printf("Syntax Error: Use of undeclared synonym <%s> for Select.\n", synonym->c_str());
        return false;
    }

    queryObject->selectSynonym = qd.value();
    return true;
}

bool QueryParser::isQueryClauseValid(string type, string left, string right) {
    // Upholds the following
    /*
    Follows : 'Follows' '(' stmtRef ',' stmtRef ')'
    FollowsT : 'Follows*' '(' stmtRef ',' stmtRef ')'

    Parent : 'Parent' '(' stmtRef ',' stmtRef ')'
    ParentT : 'Parent*' '(' stmtRef ',' stmtRef ')'

    UsesS : 'Uses' '(' stmtRef ',' entRef ')'
    UsesP : 'Uses' '(' entRef ',' entRef ')'

    ModifiesS : 'Modifies' '(' stmtRef ',' entRef ')'
    ModifiesP : 'Modifies' '(' entRef ',' entRef ')'
     */

    if (type == "Follows" || type == "Follows*" ||
        type == "Parent" || type == "Parent*" ) {
        return lex->isStmtRef(left) && lex->isStmtRef(right);
    }

    if (type == "Uses" || type == "Modifies") {
        return (lex->isStmtRef(left) && lex->isEntRef(right)) ||
                (lex->isEntRef(left) && lex->isEntRef(right));
    }
    return false;
}

bool QueryParser::isDeclared(string synonym) {
    optional<QueryDeclaration> lqd = findMatchingDeclaration(synonym);
    return lqd.has_value();
}

bool QueryParser::isDeclaredProcedure(string synonym) {
    optional<QueryDeclaration> posProc = findMatchingDeclaration(synonym);
    return posProc.has_value() && posProc.value().type == QueryDeclaration::PROCEDURE;
}

optional<string> QueryParser::parseClauseType() {
    optional<string> clause = lex->nextClause();
    if (clause == nullopt) {
        string cl_bad = lex->nextToken();
        if (cl_bad.empty()) {
            printf("Syntax Error: Clause expected after \"such that\"\n");
        } else {
            printf("Syntax Error: Invalid clause <%s>\n", cl_bad.c_str());
        }
    }
    return clause;
}

bool QueryParser::lookForClauseGrammarSymbol(string symbol, string notFoundMessage) {
    optional<string> s = lex->nextExpected(symbol);
    if (s == nullopt) {
        printf("%s", &notFoundMessage);
        return false;
    }
    return true;
}

optional<string> QueryParser::parseClauseVariable(string clause) {
    optional<string> var = lex->nextClauseVariable();
    if (var == nullopt) {
        printf("Syntax Error: No clause variable found.\n");
        return nullopt;
    }
    if (lex->isValidSynonym(var->c_str())) { // check if synonym has been
        if (!isDeclared(var->c_str())) {
            printf("Syntax Error: Use of undeclared synonym <%s> for clause %s\n", var->c_str(), clause.c_str());
            return nullopt;
        }
    }
    return var;
}

bool QueryParser::parseClause() {
    if (lex->isEndOfQuery()) {
        printf("Query Incomplete. Expected a clause.\n");
        return false;
    }
    optional<string> clause = parseClauseType();
    if (clause == nullopt) return false;
    lookForClauseGrammarSymbol("(", "Syntax Error: Expected '(' for clause declaration.\n");
    // left variable
    optional<string> left = parseClauseVariable(clause->c_str());
    if (left == nullopt) return false;
    lookForClauseGrammarSymbol(",", "Syntax Error: Expected ',' for clause parameter delimiter.\n");
    // right variable
    optional<string> right = parseClauseVariable(clause->c_str());
    if (right == nullopt) return false;
    lookForClauseGrammarSymbol(")", "Syntax Error: Expected ')' for end of clause declaration\n");
    return buildClause(clause->c_str(), left->c_str(), right->c_str());
}

bool QueryParser::buildClause(string clause, string left, string right) {
    bool clauseValid = isQueryClauseValid(clause, left, right);

    if (clauseValid) {
        QueryClause::clause_type type = determineClauseType(clause, left, right);
        ClauseVariable::variable_type leftType = determineVariableType(left);
        string l = left;

        if (ClauseVariable::variable_type::identifier == leftType) {
            l = l.substr(1, l.length() - 2);
        }
        ClauseVariable lcv(leftType, l, determineDeclarationType(l));
        ClauseVariable::variable_type rightType = determineVariableType(right);
        string r = right;

        if (ClauseVariable::variable_type::identifier == rightType) {
            r = r.substr(1, r.length() - 2);
        }

        ClauseVariable rcv(rightType, r, determineDeclarationType(r));
        QueryClause clause(type, lcv, rcv);
        queryObject->clauses.push_back(clause);
    } else {
        return false;
    }

    return true;
}

ClauseVariable::variable_type QueryParser::determineVariableType(string w) {
    if (w == "_")
        return ClauseVariable::wildcard;
    if (lex->isIdentifier(w))
        return ClauseVariable::identifier;
    if (lex->isInteger(w))
        return ClauseVariable::integer;
    if (lex->isValidSynonym(w))
        return ClauseVariable::synonym;
}

QueryClause::clause_type QueryParser::determineClauseType(string type, string left, string right) {
    if (type == "Follows")
        return QueryClause::follows;
    if (type == "Follows*")
        return QueryClause::followsT;
    if (type == "Parent")
        return QueryClause::parent;
    if (type == "Parent*")
        return QueryClause::parentT;
    if (type == "Uses" && lex->isStmtRef(left) && lex->isEntRef(right))
        return isDeclaredProcedure(left) ? QueryClause::usesP : QueryClause::usesS;
    if (type == "Uses" && lex->isEntRef(left) && lex->isEntRef(right))
        return QueryClause::usesP;
    if (type == "Modifies" && lex->isStmtRef(left) && lex->isEntRef(right))
        return isDeclaredProcedure(left) ? QueryClause::modifiesP : QueryClause::modifiesS;
    if (type == "Modifies" && lex->isEntRef(left) && lex->isEntRef(right))
        return QueryClause::modifiesP;
}

bool QueryParser::isValidPatternSynType(QueryDeclaration declared) {
    return declared.type != QueryDeclaration::ASSIGN &&
        declared.type != QueryDeclaration::WHILE &&
        declared.type != QueryDeclaration::IF;
}

optional<QueryDeclaration> QueryParser::parsePatternSyn() {
    optional<string> patternSyn = lex->nextSynonym();
    if (patternSyn == nullopt) {
        printf("Syntax Error: No pattern synonym was found\n");
        return nullopt;
    }
    optional<QueryDeclaration> declared = findMatchingDeclaration(patternSyn->c_str());
    if (declared == nullopt) {
        printf("Syntax Error: Use of undeclared pattern synonym <%s>\n", patternSyn->c_str());
        return nullopt;
    } else {
        if (isValidPatternSynType(declared.value())) {
            printf("Pattern synonym must be of type assign, while or if");
            return nullopt;
        }
    }
    return declared;
}

optional<string> QueryParser::parsePatternLHS() {
    optional<string> lhs = lex->nextClauseVariable();
    if (lhs == nullopt) {
        printf("Syntax Error: No clause variable found for pattern LHS.\n");
        return nullopt;
    }
    if (!(lex->isWildCard(lhs->c_str()) || lex->isIdentifier(lhs->c_str()) || lex->isValidSynonym(lhs->c_str()))) {
        return nullopt;
    }
    if (lex->isValidSynonym(lhs->c_str())) {
        optional<QueryDeclaration> lhsDeclared = findMatchingDeclaration(lhs->c_str());
        if (lhsDeclared == nullopt) {
            printf("Syntax Error: Use of undeclared pattern LHS synonym <%s>\n", lhs->c_str());
            return nullopt;
        }
    }
    return lhs;
}

PatternVariable *QueryParser::parsePatternRHS() {
    TNode *miniAST = nullptr;
    PatternVariable::pattern_type pt = PatternVariable::wildcard;

    optional<string> patternExpr = lex->nextPatternExpression();
    if (patternExpr != nullopt) {
        if (patternExpr == "_") {
            pt = PatternVariable::wildcard;
        } else {
            Parser simpleParser;
            string expr = patternExpr->c_str();

            if (expr.at(0) == '_' && expr.at(expr.length() - 1) == '_') {
                pt = PatternVariable::subpattern; // sub pattern
                expr = expr.substr(1, expr.length() - 2);
            } else if (expr.at(0) == '"' && expr.at(expr.length() - 1) == '"') {
                pt = PatternVariable::fullpattern; // full pattern
                expr = expr.substr(1, expr.length() - 2);
            } else {
                printf("Unknown pattern RHS: <%s>\n", patternExpr->c_str());
                return nullptr;
            }
            try {
                miniAST = simpleParser.parseExpr(expr);
            } catch (exception &e) {
                miniAST = nullptr;
            }
            if (miniAST == nullptr) {
                printf("Invalid pattern RHS: <%s>\n", patternExpr->c_str());
                return nullptr;
            }
        }
        PatternVariable *pv = new PatternVariable(pt, miniAST);
        return pv;
    } else {
        // Invalid RHS
        printf("Syntax Error: No valid pattern expression found in RHS.\n");
        return nullptr;
    }
}

bool QueryParser::parsePatternClause() {
    optional<string> patt = lex->nextExpected("pattern");
    if (!patt.has_value()) {
        printf("Malformed input. Expected keyword: pattern\n");
        return false;
    }
    optional<QueryDeclaration> patternSyn = parsePatternSyn();
    if (patternSyn == nullopt) return false;
    lookForClauseGrammarSymbol("(", "Syntax Error: Expected '(' for pattern declaration parameters\n");
    optional<string> lhs = parsePatternLHS();
    if (lhs == nullopt) return false;
    lookForClauseGrammarSymbol(",", "Syntax Error: Expected ',' for pattern parameter delimiter.\n");
    if (lex->isEndOfQuery()) {
        printf("Expected a pattern expression.\n");
        return false;
    }
    PatternVariable* pv = parsePatternRHS();
    if (pv == nullptr) return false;
    lookForClauseGrammarSymbol(")", "Syntax Error: Expected ')' for end of pattern declaration.\n");
    buildPatternClauseObject(patternSyn.value(), lhs->c_str(), pv);
    return true;
}

void QueryParser::buildPatternClauseObject(QueryDeclaration patternSyn, string lhs, PatternVariable *rhs) {
    ClauseVariable::variable_type leftType = determineVariableType(lhs);
    string l = lhs;
    if (ClauseVariable::variable_type::identifier == leftType) {
        l = l.substr(1, l.length() - 2);
    }
    ClauseVariable lcv(leftType, l, determineDeclarationType(l));
    PatternClause pc(patternSyn, lcv, *rhs);
    queryObject->patternClauses.push_back(pc);
}

QueryParser::QueryParser(string &input) : input(input) {}

QueryObject *QueryParser::parse() {
    // Base declaration
    vector<QueryDeclaration> declarations;
    vector<QueryClause> clauses;
    vector<PatternClause> patternClauses;
    string a = "";
    QueryDeclaration s(QueryDeclaration::ASSIGN, a);
    queryObject = new QueryObject(declarations, clauses, patternClauses, s, false);

    lex = new QueryLexer(input);

    if (!parseDeclarations()) {
        cleanup();
        return queryObject;
    }

    if (!parseSelectSynonym()) {
        cleanup();
        return queryObject;
    }

    while (!lex->isEndOfQuery()) {
        if (lex->peekNextIsString("such")) {
            skipSuchThat();
            if (!parseClause()) {
                cleanup();
                return queryObject;
            }
        } else if (lex->peekNextIsString("pattern")) {
            // Do pattern stuff
            if (!parsePatternClause()) {
                queryObject->isQueryValid = false;
                cleanup();
                return queryObject;
            }
        } else {
            queryObject->isQueryValid = false;
            string unexpected = lex->nextToken();
            printf("Syntax Error: Unexpected term <%s> found.", unexpected.c_str());
            cleanup();
            return queryObject;
        }
    }

    printf("Query parsed.\n");
    queryObject->isQueryValid = true;
    cleanup();
    return queryObject;
}

void QueryParser::cleanup() {
    delete lex;
    lex = nullptr;
}

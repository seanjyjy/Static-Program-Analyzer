#include <cstdio>
#include <stdexcept>
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

    while (!lex->peekNextIsString("Select")) {
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
    };
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
    Entities* t = QueryDeclaration::stringToType(type);
    QueryDeclaration qd(t, synonym);
    queryObject->getDeclarations().push_back(qd);
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
    for (QueryDeclaration qd : queryObject->getDeclarations()) {
        if (qd.synonym == synonym)
            return qd;
    }
    return nullopt;
}

Entities* QueryParser::determineDeclarationType(string synonym) {
    optional<QueryDeclaration> qd = findMatchingDeclaration(synonym);
    if (qd == nullopt) {
        // not a declared synonym
        return NoneEntities::getInstance();
    } else {
        return qd->type;
    }
}

bool QueryParser::parseSelectTuple() {
    lookForClauseGrammarSymbol("<", "Syntax Error: Expected '<' for tuple beginning\n");
    if (!parseSelectSingle()) {
        return false;
    }

    while(lex->peekNextIsString(",")) {
        lookForClauseGrammarSymbol(",", "Syntax Error: Expected ',' for tuple delimiting\n");
        if (!parseSelectSingle()) {
            return false;
        }
    }

    lookForClauseGrammarSymbol(">", "Syntax Error: Expected '>' for tuple end.\n");

    return true;
}

bool QueryParser::parseSelectSingle() {
    optional<string> synonym = lex->nextSynonym();
    if (!synonym.has_value()) {
        printf("Syntax Error: Use of select with no synonym.\n");
        return false;
    }

    optional<QueryDeclaration> qd = findMatchingDeclaration(synonym.value());
    if (!qd.has_value()) {
        queryObject->setUseOfUndeclaredVariable(true);
        printf("Semantic Error: Use of undeclared synonym <%s> for Select.\n", synonym->c_str());
        qd = {NoneEntities::getInstance(), synonym.value()};
    }

    ///// LEGACY CODE FROM ITER 1 (todo remove)
    // queryObject->selectSynonym = qd.value();
    ///// LEGACY CODE ENDS

    if (lex->peekNextIsString(".")) {
        lookForClauseGrammarSymbol(".", "Syntax Error: Expected '.' for attributes\n");
        Selectable::attributeName attr = parseSelectAttribute();
        if (attr == Selectable::NONE) {
            return false;
        }
        Selectable s(Selectable::ATTR_REF, qd.value(), attr);
        queryObject->getSelectTarget().addSelectable(s);
        return true;
    }

    // No attribute, just synonym
    Selectable s(Selectable::SYNONYM, qd.value(), Selectable::NONE);
    queryObject->getSelectTarget().addSelectable(s);
    return true;
}

Selectable::attributeName QueryParser::parseSelectAttribute() {
    optional<string> attr = lex->nextAttribute();
    if (!attr.has_value()) {
        printf("Syntax Error: Invalid attribute was specified.\n");
        return Selectable::NONE;
    }
    string aT = attr.value();
    if (aT == "procName") {
        return Selectable::PROC_NAME;
    } else if (aT == "varName") {
        return Selectable::VAR_NAME;
    } else if (aT == "value") {
        return Selectable::VALUE;
    } else if (aT == "stmt#") {
        return Selectable::STMT_NUM;
    }
    throw runtime_error("Internal Error: Invalid select attribute type");
}

bool QueryParser::parseSelectTarget() {
    lex->nextExpected("Select");

    if (findMatchingDeclaration("BOOLEAN") == nullopt && lex->peekNextIsString("BOOLEAN")) {
        lex->nextExpected("BOOLEAN");
        queryObject->getSelectTarget() = SelectTarget(SelectTarget::BOOLEAN);
        return true;
    }

    queryObject->getSelectTarget() = SelectTarget(SelectTarget::TUPLE);

    if (lex->peekNextIsString("<")) {
        return parseSelectTuple();
    }

    return parseSelectSingle();
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

    Calls : 'Calls' '(' entRef ',' entRef ')'
    CallsT : 'Calls*' '(' entRef ',' entRef ')'

    Next : 'Next' '(' stmtRef ',' stmtRef ')'
    NextT : 'Next*' '(' stmtRef ',' stmtRef ')'

    Affects : 'Affects' '(' stmtRef ',' stmtRef ')'
    AffectsT : 'Affects*' '(' stmtRef ',' stmtRef ')'
     */

    if (type == "Follows" || type == "Follows*" ||
        type == "Parent" || type == "Parent*" ||
        type == "Next" || type == "Next*" ||
        type == "Affects" || type == "Affects*" ) {
        return lex->isStmtRef(left) && lex->isStmtRef(right);
    }

    if (type == "Calls" || type == "Calls*") {
        return lex->isEntRef(left) && lex->isEntRef(right);
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
    return posProc.has_value() && posProc.value().type->isProcedure();
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
        printf("%s", notFoundMessage.c_str());
        return false;
    }
    return true;
}

optional<string> QueryParser::parseClauseVariable(string clause) {
    optional<string> var = lex->nextClauseVariable();
    if (var == nullopt) {
        printf("Syntax Error: No valid clause variable found.\n");
        return nullopt;
    }
    if (lex->isValidSynonym(var->c_str())) { // check if synonym has been
        if (!isDeclared(var->c_str())) {
            queryObject->setUseOfUndeclaredVariable(true);
            printf("Semantic Error: Use of undeclared synonym <%s> for clause %s\n", var->c_str(), clause.c_str());
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

bool QueryParser::buildClause(string clauseStr, string left, string right) {
    bool clauseValid = isQueryClauseValid(clauseStr, left, right);

    if (clauseValid) {
        QueryClause::clause_type type = determineClauseType(clauseStr, left, right);

        ClauseVariable::variable_type leftType = determineVariableType(left);
        string l = left;
        if (leftType == ClauseVariable::variable_type::identifier) {
            l = l.substr(1, l.length() - 2);
        }
        ClauseVariable lcv;
        if (leftType == ClauseVariable::synonym) {
            optional<QueryDeclaration> qd = findMatchingDeclaration(l);
            if (qd.has_value()) {
                lcv = ClauseVariable(leftType, l, qd.value());
            } else {
                lcv = ClauseVariable(leftType, l, {NoneEntities::getInstance(), l});
            }
        } else {
            lcv = ClauseVariable(leftType, l, determineDeclarationType(l));
        }

        ClauseVariable::variable_type rightType = determineVariableType(right);
        string r = right;
        if (rightType == ClauseVariable::variable_type::identifier) {
            r = r.substr(1, r.length() - 2);
        }
        ClauseVariable rcv;
        if (rightType == ClauseVariable::synonym) {
            optional<QueryDeclaration> qd = findMatchingDeclaration(r);
            if (qd.has_value()) {
                rcv = ClauseVariable(rightType, r, qd.value());
            } else {
                rcv = ClauseVariable(rightType, r, {NoneEntities::getInstance(), r});
            }
        } else {
            rcv = ClauseVariable(rightType, r, determineDeclarationType(r));
        }

        QueryClause *queryClause = new QueryClause(type, lcv, rcv);
        queryObject->getClauses().push_back(*queryClause);

        SuperClause *super = new SuperClause(*queryClause);
        queryObject->getSuperClauses().push_back(super);
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
    throw runtime_error("Internal Error: Invalid variable type");
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
    if (type == "Calls")
        return QueryClause::calls;
    if (type == "Calls*")
        return QueryClause::callsT;
    if (type == "Next")
        return QueryClause::next;
    if (type == "Next*")
        return QueryClause::nextT;
    if (type == "Affects")
        return QueryClause::affects;
    if (type == "Affects*")
        return QueryClause::affectsT;
    if (type == "Uses" && lex->isStmtRef(left) && lex->isEntRef(right))
        return lex->isWildCard(left) ? QueryClause::generic_uses : isDeclaredProcedure(left) ? QueryClause::usesP : QueryClause::usesS;
    if (type == "Uses" && lex->isEntRef(left) && lex->isEntRef(right))
        return QueryClause::usesP;
    if (type == "Modifies" && lex->isStmtRef(left) && lex->isEntRef(right))
        return lex->isWildCard(left) ? QueryClause::generic_modifies : isDeclaredProcedure(left) ? QueryClause::modifiesP : QueryClause::modifiesS;
    if (type == "Modifies" && lex->isEntRef(left) && lex->isEntRef(right))
        return QueryClause::modifiesP;
    throw runtime_error("Internal Error: Invalid clause type");
}

optional<QueryDeclaration> QueryParser::parsePatternSyn() {
    optional<string> patternSyn = lex->nextSynonym();
    if (patternSyn == nullopt) {
        printf("Syntax Error: No pattern synonym was found\n");
        return nullopt;
    }
    optional<QueryDeclaration> declared = findMatchingDeclaration(patternSyn->c_str());
    if (declared == nullopt) {
        queryObject->setUseOfUndeclaredVariable(true);
        printf("Semantic Error: Use of undeclared pattern synonym <%s>\n", patternSyn->c_str());
        declared = {NoneEntities::getInstance(), patternSyn.value()};
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
            queryObject->setUseOfUndeclaredVariable(true);
            printf("Semantic Error: Use of undeclared pattern LHS synonym <%s>\n", lhs->c_str());
        }
    }
    return lhs;
}

optional<PatternVariable> QueryParser::parsePatternRHS() {
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
                return nullopt;
            }
            try {
                miniAST = simpleParser.parseExpr(expr);
            } catch (exception &) {
                miniAST = nullptr;
            }
            if (miniAST == nullptr) {
                printf("Invalid pattern RHS: <%s>\n", patternExpr->c_str());
                return nullopt;
            }
        }
        return PatternVariable(pt, miniAST, patternExpr->c_str());
    } else {
        // Invalid RHS
        printf("Syntax Error: No valid pattern expression found in RHS.\n");
        return nullopt;
    }
}

bool QueryParser::skipPattern() {
    optional<string> patt = lex->nextExpected("pattern");
    if (!patt.has_value()) {
        printf("Malformed input. Expected keyword: pattern\n");
        return false;
    }
    return true;
}

bool QueryParser::skipWith() {
    optional<string> w = lex->nextExpected("with");
    if (!w.has_value()) {
        printf("Malformed input. Expected keyword: with\n");
        return false;
    }
    return true;
}

bool QueryParser::parsePatternClause() {
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
    optional<vector<PatternVariable>> pv;
    Entities *type = patternSyn->type;
    if (type->isAssign()) {
        pv = parseAssignPatternParams();
    } else if (type->isIf()) {
        pv = parseIfPatternParams();
    } else if (type->isWhile()) {
        pv = parseWhilePatternParams();
    } else {
        pv = parseDummyPatternParams();
    }
    if (pv == nullopt) return false;
    lookForClauseGrammarSymbol(")", "Syntax Error: Expected ')' for end of pattern declaration.\n");
    buildPatternClauseObject(patternSyn.value(), lhs->c_str(), *pv);
    return true;
}

optional<vector<PatternVariable>> QueryParser::parseDummyPatternParams() {
    optional<PatternVariable> pv = parsePatternRHS();
    if (pv == nullopt) return nullopt;
    if (!pv->isWildcard()) {
        delete pv->getMiniAST();
        return nullopt;
    }
    // Found an if-pattern like query
    if (lex->peekNextIsString(",")) {
        lex->nextExpected(",");
        if (lex->isEndOfQuery()) {
            printf("Expected 2 pattern expression.\n");
            return nullopt;
        }
        optional<PatternVariable> elsePv = parsePatternRHS();
        if (elsePv == nullopt) {
            delete pv->getMiniAST();
            return nullopt;
        }
        if (!elsePv->isWildcard()) {
            delete pv->getMiniAST();
            delete elsePv->getMiniAST();
            return nullopt;
        }
    }

    // return dummy pattern data cause inconsequential
    return vector<PatternVariable>({});
}

optional<vector<PatternVariable>> QueryParser::parseAssignPatternParams() {
    optional<PatternVariable> pv = parsePatternRHS();
    if (pv == nullopt) return nullopt;
    return vector<PatternVariable>({*pv});
}

optional<vector<PatternVariable>> QueryParser::parseIfPatternParams() {
    optional<PatternVariable> thenPv = parsePatternRHS();
    if (thenPv == nullopt) return nullopt;
    if (!thenPv->isWildcard()) {
        delete thenPv->getMiniAST();
        return nullopt;
    }
    lookForClauseGrammarSymbol(",", "Syntax Error: Expected ',' for pattern parameter delimiter.\n");
    if (lex->isEndOfQuery()) {
        printf("Expected 2 pattern expression.\n");
        return nullopt;
    }
    optional<PatternVariable> elsePv = parsePatternRHS();
    if (elsePv == nullopt) {
        delete thenPv->getMiniAST();
        return nullopt;
    }
    if (!elsePv->isWildcard()) {
        delete thenPv->getMiniAST();
        delete elsePv->getMiniAST();
        return nullopt;
    }
    return vector<PatternVariable>({*thenPv, *elsePv});
}

optional<vector<PatternVariable>> QueryParser::parseWhilePatternParams() {
    optional<PatternVariable> pv = parsePatternRHS();
    if (pv == nullopt) return nullopt;
    if (!pv->isWildcard()) {
        delete pv->getMiniAST();
        return nullopt;
    }
    return vector<PatternVariable>({*pv});
}

void QueryParser::buildPatternClauseObject(QueryDeclaration patternSyn, string lhs, vector<PatternVariable> rhs) {
    ClauseVariable::variable_type leftType = determineVariableType(lhs);
    string l = lhs;
    if (ClauseVariable::variable_type::identifier == leftType) {
        l = l.substr(1, l.length() - 2);
    }

    ClauseVariable lcv;
    if (leftType == ClauseVariable::synonym) {
        // lcv = ClauseVariable(leftType, l, findMatchingDeclaration(l).value());
        optional<QueryDeclaration> qd = findMatchingDeclaration(l);
        if (qd.has_value()) {
            lcv = ClauseVariable(leftType, l, qd.value());
        } else {
            lcv = ClauseVariable(leftType, l, {NoneEntities::getInstance(), l});
        }
    } else {
        auto declarationType = determineDeclarationType(l);
        lcv = ClauseVariable(leftType, l, declarationType);
    }

    PatternClause *pc = new PatternClause(patternSyn, lcv, rhs);
    queryObject->getPatternClauses().push_back(*pc);

    SuperClause *super = new SuperClause(*pc);
    queryObject->getSuperClauses().push_back(super);
}

QueryParser::QueryParser(string &input) : input(input) {}

bool QueryParser::parseSuchThatClauses() {
    if (!skipSuchThat()) {
        return false;
    }
    do {
        if (!parseClause()) {
            return false;
        }
    } while (lex->peekNextIsString("and") && lex->nextExpected("and"));
    return true;
}

bool QueryParser::parsePatternClauses() {
    skipPattern();
    do {
        if (!parsePatternClause()) {
            return false;
        }
    } while (lex->peekNextIsString("and") && lex->nextExpected("and"));
    return true;
}

bool QueryParser::parseWithClauses() {
    skipWith();
    do {
        if (!parseWithClause()) {
            return false;
        }
    } while (lex->peekNextIsString("and") && lex->nextExpected("and"));
    return true;
}

bool QueryParser::parseWithClause() {
    optional<WithVariable> left = parseWithRef();
    if (left == nullopt) {
        return false;
    }
    lookForClauseGrammarSymbol("=", "Syntax Error: Expected '=' for with clause, following first ref\n");
    optional<WithVariable> right = parseWithRef();
    if (right == nullopt) {
        return false;
    }
    WithClause* wc = new WithClause(left.value(), right.value());
    queryObject->getWithClauses().push_back(*wc);

    SuperClause *super = new SuperClause(*wc);
    queryObject->getSuperClauses().push_back(super);
    return true;
}

// todo: build the with clause somehow
optional<WithVariable> QueryParser::parseWithRef() {
    if (lex->peekNextIsString("\"")) {
        // '"' IDENT '"'
        string ident = lex->nextToken();
        if (lex->isIdentifier(ident)) {
            return WithVariable(ident.substr(1, ident.length() - 2), WithVariable::IDENT);
        } else {
            return nullopt;
        }
    }
    string n = lex->nextToken();
    if (lex->isInteger(n)) {
        // INTEGER
        return WithVariable(n, WithVariable::INTEGER);
    } else if (lex->isValidSynonym(n)) {
        // attrRef : synonym '.' attrName
        optional<QueryDeclaration> syn = findMatchingDeclaration(n);
        if (!syn.has_value()) {
            queryObject->setUseOfUndeclaredVariable(true);
            printf("Semantic Error: Use of undeclared synonym <%s> for Select.\n", n.c_str());

            syn = {NoneEntities::getInstance(), n};
        }
        if (lex->peekNextIsString(".") && lex->nextExpected(".")) {
            string attr = lex->nextToken();
            if (isValidAttrName(attr)) {
                return WithVariable(toAttrName(attr), syn.value());
            } else {
                printf("Syntax Error: Invalid attribute name <%s> found.\n", attr.c_str());
                return nullopt;
            }
        } else {
            printf("Syntax Error: Expected an attribute reference denoted by '.'\n");
            return nullopt;
        }
    } else {
        printf("Syntax Error: Invalid ref term <%s> found.\n", n.c_str());
        return nullopt;
    }
}

WithVariable::attributeName QueryParser::toAttrName(string w) {
    if (w == "procName") {
        return WithVariable::PROC_NAME;
    } else if (w == "varName") {
        return WithVariable::VAR_NAME;
    } else if (w == "value") {
        return WithVariable::VALUE;
    } else if (w == "stmt#") {
        return WithVariable::STMT_NUM;
    } else {
        return WithVariable::NONE;
    }
}

bool QueryParser::isValidAttrName(string w) {
    // 'procName'| 'varName' | 'value' | 'stmt#'
    return w == "procName" || w == "varName" || w == "value" || w == "stmt#";
}

QueryObject *QueryParser::parse() {
    // Base declaration
    vector<QueryDeclaration> declarations;
    vector<QueryClause> clauses;
    vector<PatternClause> patternClauses;
    vector<WithClause> withClauses;
    vector<SuperClause*> superClauses;
    string a = "";
    QueryDeclaration s(nullptr, a);
    SelectTarget st(SelectTarget::BOOLEAN);
    queryObject = new QueryObject(declarations, clauses, patternClauses, withClauses, superClauses, s, st, true);
    lex = new QueryLexer(input);

    if (!parseDeclarations()) {
        queryObject->setValidity(false);
        cleanup();
        return queryObject;
    }

    if (!parseSelectTarget()) {
        queryObject->setValidity(false);
        cleanup();
        return queryObject;
    }

    while (!lex->isEndOfQuery()) {
        if (lex->peekNextIsString("such")) {
            if (!parseSuchThatClauses()) {
                cleanup();
                queryObject->setValidity(false);
                return queryObject;
            }
        } else if (lex->peekNextIsString("pattern")) {
            if (!parsePatternClauses()) {
                queryObject->setValidity(false);
                cleanup();
                return queryObject;
            }
        } else if (lex->peekNextIsString("with")) {
            if (!parseWithClauses()) {
                queryObject->setValidity(false);
                cleanup();
                return queryObject;
            }
        } else {
            queryObject->setValidity(false);
            string unexpected = lex->nextToken();
            printf("Syntax Error: Unexpected term <%s> found.\n", unexpected.c_str());
            cleanup();
            return queryObject;
        }
    }

    if (queryObject->hasUseOfUndeclaredVariable())
        printf("Query parsed but contains semantic errors\n");
    else
        printf("Query parsed.\n");
    cleanup();
    return queryObject;
}

void QueryParser::cleanup() {
    delete lex;
    lex = nullptr;
}


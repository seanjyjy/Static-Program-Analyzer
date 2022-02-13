#include <stdio.h>

#include "QueryParser.h"

using namespace std;

bool QueryParser::skipSuchThat() {
    optional<string> such = lex->nextSpecial("such");
    if (!such.has_value()) {
        printf("Malformed input. Expected keyword: such\n");
        return false;
    }
    optional<string> that = lex->nextSpecial("that");
    if (!that.has_value()) {
        printf("Malformed input. Expected keyword: that\n");
        return false;
    }
    return true;
}

bool QueryParser::parseDeclarations() {

    bool isDeclarationEnd = false;

    do {
        if (lex->isEndOfQuery()) {
            printf("Query Incomplete.");
            return false;
        }
        optional<string> type = lex->nextDeclarationType();
        if (type == nullopt) {
            return false;
        }
        optional<string> synonym = lex->nextSynonym();
        if (type == nullopt) {
            return false;
        }
        optional<string> smc = lex->nextSpecialExpected(";");
        if (smc == nullopt) {
            return false;
        }

        // Populate queryObject
        string syn = synonym->c_str();
        QueryDeclaration::design_entity_type t = QueryDeclaration::stringToType(type.value());
        QueryDeclaration qd(t, syn);
        queryObject->declarations.push_back(qd);

        isDeclarationEnd = lex->peekNextIsString("Select");
    } while (!isDeclarationEnd);

    return true;
}

optional<QueryDeclaration> QueryParser::findMatchingDeclaration(string synonym) {
    for (QueryDeclaration qd : queryObject->declarations) {
        if (qd.synonym == synonym)
            return qd;
    }
    return nullopt;
}

bool QueryParser::parseSelectSynonym() {
    lex->nextSpecial("Select");
    optional<string> synonym = lex->nextSynonym();
    optional<QueryDeclaration> qd = findMatchingDeclaration(synonym->c_str());
    if (!qd.has_value()) {
        // todo: throw something?
        printf("Use of undeclared synonym for Select\n");
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

bool QueryParser::parseClause() {
    if (lex->isEndOfQuery()) {
        printf("Query Incomplete. Expected a clause.\n");
        return false;
    }
    // Get the clause type
    optional<string> clause = lex->nextClause();
    if (clause == nullopt) {
        return false;
    }
    optional<string> lp = lex->nextSpecialExpected("(");
    if (lp == nullopt) {
        return false;
    }
    // left variable
    optional<string> left = lex->nextClauseVariable();
    if (left == nullopt) {
        return false;
    }
    if (lex->isValidSynonym(left->c_str())) { // check if synonym has been
        if (!isDeclared(left->c_str())) {
            printf("Use of undeclared synonym <%s> for clause: %s\n", left->c_str(), clause->c_str());
            return false;
        }
    }

    optional<string> c = lex->nextSpecialExpected(",");
    if (c == nullopt) {
        return false;
    }
    // right variable
    optional<string> right = lex->nextClauseVariable();
    if (right == nullopt) {
        return false;
    }
    if (lex->isValidSynonym(right->c_str())) { // check if synonym has been
        if (!isDeclared(right->c_str())) {
            printf("Use of undeclared synonym <%s> for clause: %s\n", right->c_str(), clause->c_str());
            return false;
        }
    }
    optional<string> rp = lex->nextSpecialExpected(")");
    if (rp == nullopt) {
        return false;
    }

    return buildClause(clause->c_str(), left->c_str(), right->c_str());
}

bool QueryParser::buildClause(string clause, string left, string right) {
    bool clauseValid = isQueryClauseValid(clause, left, right);

    if (clauseValid) {
        QueryClause::clause_type type = determineClauseType(clause, left, right);
        ClauseVariable::variable_type leftType = QueryParser::determineVariableType(left);
        string l = left;

        if (ClauseVariable::variable_type::identifier == leftType) {
            l = l.substr(1, l.length() - 2);
        }

        ClauseVariable lcv(leftType, l);
        ClauseVariable::variable_type rightType = QueryParser::determineVariableType(right);
        string r = right;

        if (ClauseVariable::variable_type::identifier == rightType) {
            r = r.substr(1, r.length() - 2);
        }

        ClauseVariable rcv(rightType, r);
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

bool QueryParser::parsePatternClause() {
    optional<string> such = lex->nextSpecial("pattern");
    if (!such.has_value()) {
        printf("Malformed input. Expected keyword: pattern\n");
        return false;
    }

    optional<string> patternSyn = lex->nextSynonym();
    optional<QueryDeclaration> declared = findMatchingDeclaration(patternSyn->c_str());
    if (declared == nullopt) {
        printf("Use of undeclared pattern synonym: %s", patternSyn->c_str());
        return false;
    } else {
        if (declared->type != QueryDeclaration::ASSIGN && declared->type != QueryDeclaration::WHILE && declared->type != QueryDeclaration::IF) {
            printf("Pattern synonym must be of type assign, while or if");
            return false;
        }
    }

    if (patternSyn == nullopt) return false;

    optional<string> lp = lex->nextSpecialExpected("(");
    if (lp == nullopt) return false;

    optional<string> lhs = lex->nextClauseVariable();
    if (lhs == nullopt) return false;
    if (!(lex->isWildCard(lhs->c_str()) || lex->isIdentifier(lhs->c_str()))) {
        return false;
    }

    optional<string> c = lex->nextSpecialExpected(",");
    if (c == nullopt) return false;

    optional<string> patternExpr = lex->nextPatternExpression();
    if (patternExpr != nullopt) {
        if (patternExpr == "_") {
        } else {
            Parser simpleParser;
            string expr = patternExpr->c_str();
            TNode *miniAST = simpleParser.parseExpr(expr);
            if (miniAST == nullptr) {
                printf("Invalid pattern RHS: <%s>\n", patternExpr->c_str());
                return false;
            }
        }
    } else {
        // Invalid RHS
        return false;
    }

    return true;
}

QueryParser::QueryParser(string &input) : input(input) {}

QueryObject *QueryParser::parse() {
    // Base declaration
    vector<QueryDeclaration> declarations;
    vector<QueryClause> clauses;
    string a = "";
    QueryDeclaration s(QueryDeclaration::ASSIGN, a);
    queryObject = new QueryObject(declarations, clauses, s, false);

    lex = new QueryLexer(input);

    if (!parseDeclarations()) {
        return queryObject;
    }
    if (!parseSelectSynonym()) {
        return queryObject ;
    }

    while (!lex->isEndOfQuery()) {
        if (lex->peekNextIsString("such")) {
            skipSuchThat();
            if (!parseClause()) {
                return queryObject;
            }
        } else if (lex->peekNextIsString("pattern")) {
            // Do pattern stuff
            if (!parsePatternClause()) {

            }
            // Temp return
            queryObject->isQueryValid = true;
            return queryObject;
        } else {
            queryObject->isQueryValid = false;
            string unexpected = lex->nextToken();
            printf("Unexpected term: %s", unexpected.c_str());
            return queryObject;
        }

    }
    printf("Query parsed.\n");
    queryObject->isQueryValid = true;
    return queryObject;
//    if (lex->isEndOfQuery()) {
//        printf("Query parsed.\n");
//        queryObject->isQueryValid = true;
//        return queryObject;
//    }



    if (lex->isEndOfQuery()) {
        printf("Query parsed.\n");
        queryObject->isQueryValid = true;
        return queryObject;
    }


    // todo: Note that queries can end here. Check if ended

    //parsePatternClause();
}

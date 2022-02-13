#pragma once

#include "QPS/QueryDeclaration.h"
#include "QPS/ClauseVariable.h"

class EvaluatorUtils {
public:
    static bool isStmtType(QueryDeclaration::design_entity_type type);
    static bool isVariable(QueryDeclaration::design_entity_type type);
    static bool isProcedure(QueryDeclaration::design_entity_type type);

    // for Follow, Follow*, Parent, Parent*
    class StmtUtils {
    public:
        static bool isIntegerInteger(ClauseVariable* left, ClauseVariable* right);
        static bool isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isIntegerWildCard(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymInteger(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
        static bool isWildCardInteger(ClauseVariable* left, ClauseVariable* right);
        static bool isValidWildCardSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isWildCardWildCard(ClauseVariable* left, ClauseVariable* right);
    };

    // for ModifiesP, UsesP
    class ProcUtils {
    public:
        static bool isIdentifierIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidIdentifierSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isIdentifierWildCard(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
    };

    // For ModifiesS, UsesS
    class SUtils {
    public:
        static bool isIntegerIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isIntegerWildCard(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
    };
};
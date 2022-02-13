#pragma once

#include "QPS/QueryDeclaration.h"
#include "QPS/ClauseVariable.h"

class EvaluatorUtils {
public:
    // General utils
    static bool isStmtType(QueryDeclaration::design_entity_type type);
    static bool isModifiesStmtType(QueryDeclaration::design_entity_type type);
    static bool isUsesStmtType(QueryDeclaration::design_entity_type type);
    static bool isVariable(QueryDeclaration::design_entity_type type);
    static bool isProcedure(QueryDeclaration::design_entity_type type);
    static bool isConstant(QueryDeclaration::design_entity_type type);
    static bool isPrint(QueryDeclaration::design_entity_type type);
    static bool isRead(QueryDeclaration::design_entity_type type);
    static bool isCall(QueryDeclaration::design_entity_type type);

    static bool isIntegerSynonym(ClauseVariable *left, ClauseVariable *right);
    static bool isSynonymInteger(ClauseVariable *left, ClauseVariable *right);
    static bool isSynonymSynonym(ClauseVariable *left, ClauseVariable *right);
    static bool isIdentifierSynonym(ClauseVariable *left, ClauseVariable *right);
    static bool isWildCardSynonym(ClauseVariable *left, ClauseVariable *right);
    static bool isSynonymWildCard(ClauseVariable *left, ClauseVariable *right);
    static bool isSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

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
        static bool isValidModifiesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidUsesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);
        static bool isValidModifiesSynonymSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isValidUsesSynonymSynonym(ClauseVariable* left, ClauseVariable* right);
        static bool isValidModifiesSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
        static bool isValidUsesSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
    };
};
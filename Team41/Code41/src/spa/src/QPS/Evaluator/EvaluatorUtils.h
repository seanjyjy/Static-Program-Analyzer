#pragma once

#include "QPS/QueryDeclaration.h"
#include "QPS/ClauseVariable.h"

class EvaluatorUtils {
public:
    // General utils

    /**
     * Checks if a given query declaration is of a Statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Statement type, else false.
     */
    static bool isStmtType(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Modifies statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Modifies statement type, else false.
     */
    static bool isModifiesStmtType(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Uses statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Uses statement type, else false.
     */
    static bool isUsesStmtType(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Variable type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Variable type, else false.
     */
    static bool isVariable(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Procedure type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Procedure type, else false.
     */
    static bool isProcedure(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Constant type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Constant type, else false.
     */
    static bool isConstant(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Print type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Print type, else false.
     */
    static bool isPrint(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of an Assign type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is an Assign type, else false.
     */
    static bool isAssign(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is of a Read type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Read type, else false.
     */
    static bool isRead(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given clause variable is of a variable synonym.
     *
     * @param var Clause variable.
     * @return True if clause variable is a synonym and is also a variable, else false.
     */
    static bool isVariableSynonym(ClauseVariable *var);

    /**
     * Checks if the left clause variable is an integer and if the right clause variable is a synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is an integer and if right clause variable is a synonym else false.
     */
    static bool isIntegerSynonym(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is n synonym and if the right clause variable is an integer.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a synonym and if right clause variable is an integer else false.
     */
    static bool isSynonymInteger(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a synonym and if the right clause variable is a synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a synonym and if right clause variable is a synonym else false.
     */
    static bool isSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is an identifier and if the right clause variable is a synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is an identifier and if right clause variable is a synonym else false.
     */
    static bool isIdentifierSynonym(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a wildcard and if the right clause variable is a synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a wildcard and if right clause variable is a synonym else false.
     */
    static bool isWildCardSynonym(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a synonym and if right clause variable is a wildcard else false.
     */
    static bool isSynonymWildCard(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a synonym and if the right clause variable is an identifier.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a synonym and if right clause variable is an identifier else false.
     */
    static bool isSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a wildcard and if the right clause variable is a wildcard.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a wildcard and if right clause variable is a wildcard else false.
     */
    static bool isWildCardWildCard(ClauseVariable* left, ClauseVariable* right);

    // for Follow, Follow*, Parent, Parent*
    class StmtUtils {
    public:
        /**
         * Checks if the left clause variable is an integer and if the right clause variable is an integer.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is an integer else false.
         */
        static bool isIntegerInteger(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a wildcard else false.
         */
        static bool isIntegerWildCard(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an integer. Also it
         * checks if the left clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an integer
         * else false.
         */
        static bool isValidSynonymInteger(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a synonym. Also it
         * checks if the left clause variable and right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a valid
         * synonym else false.
         */
        static bool isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a wildcard and if the right clause variable is an integer.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a wildcard and if right clause variable is an integer else false.
         */
        static bool isWildCardInteger(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a wildcard and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a wildcard and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidWildCardSynonym(ClauseVariable* left, ClauseVariable* right);
    };

    // for ModifiesP, UsesP, Calls
    class ProcUtils {
    public:
        /**
         * Checks if the left clause variable is an identifier and if the right clause variable is an identifier.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an identifier and if right clause variable is an identifier
         * else false.
         */
        static bool isIdentifierIdentifier(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an identifier and if the right clause variable is a synonym. Also it
         * checks if teh right clause variable is a valid variable synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an identifier and if right clause variable is an valid synonym
         * else false.
         */
        static bool isValidIdentifierSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an identifier and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an identifier and if right clause variable is a wildcard else false.
         */
        static bool isIdentifierWildCard(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid procedure synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a synonym. Also it
         * checks if the left clause variable is a valid procedure synonym and if the right clause variable is a valid
         * variable synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid procedure synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
    };

    // For ModifiesS, UsesS
    class SUtils {
    public:
        /**
         * Checks if the left clause variable is an integer and if the right clause variable is an identifier.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is an identifier
         * else false.
         */
        static bool isIntegerIdentifier(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid variable synonym
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a wildcard else false.
         */
        static bool isIntegerWildCard(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid Modifies statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidModifiesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid Uses statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidUsesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a synonym. Also it
         * checks if the left clause variable is a valid Modifies statement synonym and also if the right clause
         * variable is a valid variable synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidModifiesSynonymSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a synonym. Also it
         * checks if the left clause variable is a valid Uses statement synonym and also if the right clause
         * variable is a valid variable synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidUsesSynonymSynonym(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid Modifies statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidModifiesSynonymWildCard(ClauseVariable* left, ClauseVariable* right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid Uses statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidUsesSynonymWildCard(ClauseVariable* left, ClauseVariable* right);
    };

    // For Calls, Calls*
    class CallsUtils {
    public:
        static bool isValidCallsIdentifierSynonym(ClauseVariable* left, ClauseVariable* right);

        static bool isValidCallsSynonymIdentifier(ClauseVariable* left, ClauseVariable* right);

        static bool isValidCallsSynonymSynonym(ClauseVariable* left, ClauseVariable* right);

        static bool isValidCallsSynonymWildCard(ClauseVariable* left, ClauseVariable* right);

        static bool isValidCallsWildCardSynonym(ClauseVariable* left, ClauseVariable* right);

        static bool isWildCardIdentifier(ClauseVariable* left, ClauseVariable* right);
    };
};
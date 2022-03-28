#pragma once

#include "QPS/QueryDeclaration.h"
#include "QPS/ClauseVariable.h"
#include "QPS/PatternVariable.h"
#include "QPS/Selectable.h"
#include "QPS/SelectTarget.h"
#include "PKB/PKBClient.h"
#include <optional>

// TODO shift to correct class later?
class EvaluatorUtils {
public:
    // General utils
    static bool isWithinLimit(ClauseVariable &left, ClauseVariable &right, PKBClient *pkb);

    static bool isWithinLimit(ClauseVariable &variable, PKBClient *pkb);

    /**
     * Checks if a given query declaration is of a Statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Statement type, else false.
     */
    static bool isStmtType(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is a Statement.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Statement, else false.
     */
    static bool isStmt(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is a While statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a While statement type, else false.
     */
    static bool isWhile(QueryDeclaration::design_entity_type type);

    /**
     * Checks if a given query declaration is an If statement type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a If statement type, else false.
     */
    static bool isIf(QueryDeclaration::design_entity_type type);

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
     * Checks if a given query declaration is of a Call type.
     *
     * @param type Query Declaration type.
     * @return True if query declaration is a Call type, else false.
     */
    static bool isCall(QueryDeclaration::design_entity_type type);

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


    static bool isWildCardIdentifier(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a wildcard and if the right clause variable is a wildcard.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a wildcard and if right clause variable is a wildcard else false.
     */
    static bool isWildCardWildCard(ClauseVariable *left, ClauseVariable *right);

    /**
     * Checks if the left clause variable is a wildcard and if the right clause variable is a wildcard.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return True if left clause variable is a wildcard and if right clause variable is a wildcard else false.
     */
    /**
     * Checks that declaration synonyms are unique
     *
     * @param declarations the list of declarations defined in the query
     * @return true if valid, false otherwise
     */
    static bool validateDeclarations(vector<QueryDeclaration> declarations);

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
        static bool isIntegerInteger(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidIntegerSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a wildcard else false.
         */
        static bool isIntegerWildCard(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an integer. Also it
         * checks if the left clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an integer
         * else false.
         */
        static bool isValidSynonymInteger(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a synonym. Also it
         * checks if the left clause variable and right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a valid
         * synonym else false.
         */
        static bool isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a wildcard and if the right clause variable is an integer.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a wildcard and if right clause variable is an integer else false.
         */
        static bool isWildCardInteger(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a wildcard and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a wildcard and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidWildCardSynonym(ClauseVariable *left, ClauseVariable *right);
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
        static bool isIdentifierIdentifier(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an identifier and if the right clause variable is a synonym. Also it
         * checks if teh right clause variable is a valid variable synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an identifier and if right clause variable is an valid synonym
         * else false.
         */
        static bool isValidIdentifierSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an identifier and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an identifier and if right clause variable is a wildcard else false.
         */
        static bool isIdentifierWildCard(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid procedure synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

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
        static bool isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid procedure synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right);
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
        static bool isIntegerIdentifier(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a synonym. Also it
         * checks if the right clause variable is a valid variable synonym
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a valid synonym
         * else false.
         */
        static bool isValidIntegerSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is an integer and if the right clause variable is a wildcard.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is an integer and if right clause variable is a wildcard else false.
         */
        static bool isIntegerWildCard(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid Modifies statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidModifiesSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is an identifier. Also it
         * checks if the left clause variable is a valid Uses statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is an identifier
         * else false.
         */
        static bool isValidUsesSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

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
        static bool isValidModifiesSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

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
        static bool isValidUsesSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid Modifies statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidModifiesSynonymWildCard(ClauseVariable *left, ClauseVariable *right);

        /**
         * Checks if the left clause variable is a synonym and if the right clause variable is a wildcard. Also it
         * checks if the left clause variable is a valid Uses statement synonym.
         *
         * @param left Clause variable.
         * @param right Clause variable.
         * @return True if left clause variable is a valid synonym and if right clause variable is a wildcard
         * else false.
         */
        static bool isValidUsesSynonymWildCard(ClauseVariable *left, ClauseVariable *right);
    };

    // For Calls, Calls*
    class CallsUtils {
    public:
        static bool isValidCallsIdentifierSynonym(ClauseVariable *left, ClauseVariable *right);

        static bool isValidCallsSynonymIdentifier(ClauseVariable *left, ClauseVariable *right);

        static bool isValidCallsSynonymSynonym(ClauseVariable *left, ClauseVariable *right);

        static bool isValidCallsSynonymWildCard(ClauseVariable *left, ClauseVariable *right);

        static bool isValidCallsWildCardSynonym(ClauseVariable *left, ClauseVariable *right);
    };

    class AttrUtils {
    public:
        /**
         * Checks that the select target is valid.
         * Specially checks that the attribute matches the synonym type
         *
         * @param target the select target
         * @return true if valid, false otherwise
         */
        static bool validateSelectTarget(SelectTarget *target);

        /**
         * Maps the raw data to the correct output based on attribute type
         *
         * @param target the select target
         * @param rawData the rawData
         * @param pkb the knowledge base
         * @return the correct output based on Selectable
         */
        static optional<string> getAttrFromSelectable(Selectable *target, const string &rawData, PKBClient *pkb);

        /**
         * Checks if the Selectable is a valid synonym or valid attribute ref
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isValidSelectable(Selectable *target);

        /**
         * Checks if the Selectable is of type proc.procName
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isProcProcNameAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type call.procName
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isCallProcNameAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type read.varName
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isReadVarNameAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type print.varName
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isPrintVarNameAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type var.varName
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isVarVarNameAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type const.value
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isConstantValueAttr(Selectable *target);

        /**
         * Checks if the Selectable is of type stmt.stmt#
         * @param target the select target
         * @return true if matches, false otherwise
         */
        static bool isStmtStmtNumAttr(Selectable *target);
    };

    class PatternUtils {
    public:
        static bool isWildCardWildCards(ClauseVariable variable, const vector<PatternVariable> &patternVariables,
                                        QueryDeclaration::design_entity_type type);

        static bool isIdentifierWildCards(ClauseVariable variable, const vector<PatternVariable> &patternVariables,
                                          QueryDeclaration::design_entity_type type);

        static bool isValidSynonymWildCards(ClauseVariable variable, const vector<PatternVariable> &patternVariables,
                                            QueryDeclaration::design_entity_type type);

        static bool isWildCards(const vector<PatternVariable> &patternVariables,
                                QueryDeclaration::design_entity_type type);
    };
};
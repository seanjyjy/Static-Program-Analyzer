#pragma once


#include "Common/TNode.h"

class TestParserUtils {
private:
    static TNode* parseSimple(const string& simple);
    static TNode* parseXml(const string& xml);
    static string readFile(string folder, string filename);
public:
    static void parseAssignAndCompare(const string& simple, const string& xml);
    static void parseCallAndCompare(const string& simple, const string& xml);
    static void parseCondExprAndCompare(const string& simple, const string& xml);
    static void parseConstAndCompare(const string& simple, const string& xml);
    static void parseExprAndCompare(const string& simple, const string& xml);
    static void parseIfAndCompare(const string& simple, const string& xml);
    static void parseNameAndCompare(const string& simple, const string& xml);
    static void parsePrintAndCompare(const string& simple, const string& xml);
    static void parseProcedureAndCompare(const string& simple, const string& xml);
    static void parseProgramAndCompare(const string& simple, const string& xml);
    static void parseReadAndCompare(const string& simple, const string& xml);
    static void parseTermAndCompare(const string& simple, const string& xml);
    static void parseWhileAndCompare(const string& simple, const string& xml);

    static void parseAssignExpectFailure(const string& simple);
    static void parseCallExpectFailure(const string& simple);
    static void parseCondExprExpectFailure(const string& simple);
    static void parseConstExpectFailure(const string& simple);
    static void parseExprExpectFailure(const string& simple);
    static void parseIfExpectFailure(const string& simple);
    static void parseNameExpectFailure(const string& simple);
    static void parsePrintExpectFailure(const string& simple);
    static void parseProcedureExpectFailure(const string& simple);
    static void parseProgramExpectFailure(const string& simple);
    static void parseReadExpectFailure(const string& simple);
    static void parseTermExpectFailure(const string& simple);
    static void parseWhileExpectFailure(const string& simple);
};

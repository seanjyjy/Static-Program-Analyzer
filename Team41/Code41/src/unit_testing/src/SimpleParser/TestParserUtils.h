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

    static void parseAssignExpectThrows(const string& simple);
    static void parseCallExpectThrows(const string& simple);
    static void parseCondExpectThrows(const string& simple);
    static void parseConstExpectThrows(const string& simple);
    static void parseExprExpectThrows(const string& simple);
    static void parseIfExpectThrows(const string& simple);
    static void parseNameExpectThrows(const string& simple);
    static void parsePrintExpectThrows(const string& simple);
    static void parseProcedureExpectThrows(const string& simple);
    static void parseProgramExpectThrows(const string& simple);
    static void parseReadExpectThrows(const string& simple);
    static void parseTermExpectThrows(const string& simple);
    static void parseWhileExpectThrows(const string& simple);
};

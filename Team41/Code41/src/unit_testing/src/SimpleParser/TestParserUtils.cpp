#include <filesystem>
#include <iostream>
#include "TestParserUtils.h"
#include "Common/FileReader.h"
#include "Common/AstBuilder.h"
#include "Common/TreeUtils.h"
#include "SimpleParser/Parser.h"
#include "catch.hpp"

using namespace std;

void TestParserUtils::parseAssignAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("assign", simpleFile);
    string xml = readFile("assign", xmlFile);
    Parser p;
    TNode* parsed = p.parseAssign(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseCallAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("call", simpleFile);
    string xml = readFile("call", xmlFile);
    Parser p;
    TNode* parsed = p.parseCall(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseCondExprAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("condexpr", simpleFile);
    string xml = readFile("condexpr", xmlFile);
    Parser p;
    TNode* parsed = p.parseCondExpr(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseConstAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("const", simpleFile);
    string xml = readFile("const", xmlFile);
    Parser p;
    TNode* parsed = p.parseConst(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseExprAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("expr", simpleFile);
    string xml = readFile("expr", xmlFile);
    Parser p;
    TNode* parsed = p.parseExpr(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseIfAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("if", simpleFile);
    string xml = readFile("if", xmlFile);
    Parser p;
    TNode* parsed = p.parseIf(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseNameAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("name", simpleFile);
    string xml = readFile("name", xmlFile);
    Parser p;
    TNode* parsed = p.parseName(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parsePrintAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("print", simpleFile);
    string xml = readFile("print", xmlFile);
    Parser p;
    TNode* parsed = p.parsePrint(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseProcedureAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("procedure", simpleFile);
    string xml = readFile("procedure", xmlFile);
    Parser p;
    TNode* parsed = p.parseProcedure(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseProgramAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("program", simpleFile);
    string xml = readFile("program", xmlFile);
    Parser p;
    TNode* parsed = p.parseProgram(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseReadAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("read", simpleFile);
    string xml = readFile("read", xmlFile);
    Parser p;
    TNode* parsed = p.parseRead(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseTermAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("term", simpleFile);
    string xml = readFile("term", xmlFile);
    Parser p;
    TNode* parsed = p.parseTerm(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

void TestParserUtils::parseWhileAndCompare(const string& simpleFile, const string& xmlFile) {
    string simple = readFile("while", simpleFile);
    string xml = readFile("while", xmlFile);
    Parser p;
    TNode* parsed = p.parseWhile(simple);
    TNode* expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
}

TNode *TestParserUtils::parseSimple(const string& simple) {
    Parser p;
    return p.parseProgram(simple);
}

TNode *TestParserUtils::parseXml(const string& xml) {
    return AstBuilder(xml).build();
}

string TestParserUtils::readFile(string folder, string filename) {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    vector<string> paths = {"src", "unit_testing", "simple", folder, filename};
    for (const string& p: paths) {
        cwd /= p;
    }
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    return fileContent;
}

#include <filesystem>
#include <iostream>
#include "TestParserUtils.h"
#include "Common/FileReader.h"
#include "Common/AstBuilder.h"
#include "Common/TreeUtils.h"
#include "SimpleParser/Parser.h"
#include "catch.hpp"

using namespace std;

void TestParserUtils::parseAssignAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("assign", simpleFile);
    string xml = readFile("assign", xmlFile);
    Parser p;
    TNode *parsed = p.parseAssign(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseCallAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("call", simpleFile);
    string xml = readFile("call", xmlFile);
    Parser p;
    TNode *parsed = p.parseCall(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseCondExprAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("condexpr", simpleFile);
    string xml = readFile("condexpr", xmlFile);
    Parser p;
    TNode *parsed = p.parseCondExpr(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseConstAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("const", simpleFile);
    string xml = readFile("const", xmlFile);
    Parser p;
    TNode *parsed = p.parseConst(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseExprAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("expr", simpleFile);
    string xml = readFile("expr", xmlFile);
    Parser p;
    TNode *parsed = p.parseExpr(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseIfAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("if", simpleFile);
    string xml = readFile("if", xmlFile);
    Parser p;
    TNode *parsed = p.parseIf(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseNameAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("name", simpleFile);
    string xml = readFile("name", xmlFile);
    Parser p;
    TNode *parsed = p.parseName(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parsePrintAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("print", simpleFile);
    string xml = readFile("print", xmlFile);
    Parser p;
    TNode *parsed = p.parsePrint(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseProcedureAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("procedure", simpleFile);
    string xml = readFile("procedure", xmlFile);
    Parser p;
    TNode *parsed = p.parseProcedure(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseProgramAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("program", simpleFile);
    string xml = readFile("program", xmlFile);
    Parser p;
    TNode *parsed = p.parseProgram(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseReadAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("read", simpleFile);
    string xml = readFile("read", xmlFile);
    Parser p;
    TNode *parsed = p.parseRead(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseTermAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("term", simpleFile);
    string xml = readFile("term", xmlFile);
    Parser p;
    TNode *parsed = p.parseTerm(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

void TestParserUtils::parseWhileAndCompare(const string &simpleFile, const string &xmlFile) {
    string simple = readFile("while", simpleFile);
    string xml = readFile("while", xmlFile);
    Parser p;
    TNode *parsed = p.parseWhile(simple);
    TNode *expected = parseXml(xml);
    REQUIRE(TreeUtils::isEqual(parsed, expected));
    delete parsed;
    delete expected;
}

TNode *TestParserUtils::parseSimple(const string &simple) {
    Parser p;
    return p.parseProgram(simple);
}

TNode *TestParserUtils::parseXml(const string &xml) {
    return AstBuilder(xml).build();
}

string TestParserUtils::readFile(string folder, string filename) {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    vector<string> paths = {"src", "unit_testing", "simple", folder, filename};
    for (const string &p: paths) {
        cwd /= p;
    }
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    return fileContent;
}

void TestParserUtils::parseAssignExpectFailure(const string &file) {
    string source = readFile("assign", file);
    Parser p;
    REQUIRE(p.parseAssign(source) == nullptr);
}

void TestParserUtils::parseCallExpectFailure(const string &file) {
    string source = readFile("call", file);
    Parser p;
    REQUIRE(p.parseCall(source) == nullptr);
}

void TestParserUtils::parseCondExprExpectFailure(const string &file) {
    string source = readFile("condexpr", file);
    Parser p;
    REQUIRE(p.parseCondExpr(source) == nullptr);
}

void TestParserUtils::parseConstExpectFailure(const string &file) {
    string source = readFile("const", file);
    Parser p;
    REQUIRE(p.parseConst(source) == nullptr);
}

void TestParserUtils::parseExprExpectFailure(const string &file) {
    string source = readFile("expr", file);
    Parser p;
    REQUIRE(p.parseExpr(source) == nullptr);
}

void TestParserUtils::parseIfExpectFailure(const string &file) {
    string source = readFile("if", file);
    Parser p;
    REQUIRE(p.parseIf(source) == nullptr);
}

void TestParserUtils::parseNameExpectFailure(const string &file) {
    string source = readFile("name", file);
    Parser p;
    REQUIRE(p.parseName(source) == nullptr);
}

void TestParserUtils::parsePrintExpectFailure(const string &file) {
    string source = readFile("print", file);
    Parser p;
    REQUIRE(p.parsePrint(source) == nullptr);
}

void TestParserUtils::parseProcedureExpectFailure(const string &file) {
    string source = readFile("procedure", file);
    Parser p;
    REQUIRE(p.parseProcedure(source) == nullptr);
}

void TestParserUtils::parseProgramExpectFailure(const string &file) {
    string source = readFile("program", file);
    Parser p;
    REQUIRE(p.parseProgram(source) == nullptr);
}

void TestParserUtils::parseReadExpectFailure(const string &file) {
    string source = readFile("read", file);
    Parser p;
    REQUIRE(p.parseRead(source) == nullptr);
}

void TestParserUtils::parseTermExpectFailure(const string &file) {
    string source = readFile("term", file);
    Parser p;
    REQUIRE(p.parseTerm(source) == nullptr);
}

void TestParserUtils::parseWhileExpectFailure(const string &file) {
    string source = readFile("while", file);
    Parser p;
    REQUIRE(p.parseWhile(source) == nullptr);
}

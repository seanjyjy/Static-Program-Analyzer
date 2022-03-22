#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: call") {
    SECTION("1") {
        TestParserUtils::parseCallAndCompare("1-simple.txt", "1-xml.txt");
    }SECTION("call call") {
        TestParserUtils::parseCallAndCompare("call-call.simple", "call-call.x");
    }SECTION("complex") {
        TestParserUtils::parseCallAndCompare("complex.simple", "complex.x");
    }SECTION("whitespace") {
        TestParserUtils::parseCallAndCompare("whitespace.simple", "whitespace.x");
    }SECTION("call const") {
        TestParserUtils::parseCallExpectFailure("x-call-const.simple");
    }SECTION("call incorrect name") {
        TestParserUtils::parseCallExpectFailure("x-call-name-wrong.simple");
    }
}

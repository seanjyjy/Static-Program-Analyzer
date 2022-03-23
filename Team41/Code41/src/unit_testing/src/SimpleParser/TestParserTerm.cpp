#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: term") {
    SECTION("1") {
        TestParserUtils::parseTermAndCompare("1-simple.txt", "1-xml.txt");
    }SECTION("2") {
        TestParserUtils::parseTermAndCompare("2-simple.txt", "2-xml.txt");
    }SECTION("3") {
        TestParserUtils::parseTermAndCompare("3-simple.txt", "3-xml.txt");
    }SECTION("complex") {
        TestParserUtils::parseTermAndCompare("complex.simple", "complex.x");
    }SECTION("whitespace") {
        TestParserUtils::parseTermAndCompare("whitespace.simple", "whitespace.x");
    }
}

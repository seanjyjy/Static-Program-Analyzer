#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: expression") {
    SECTION("1") {
        TestParserUtils::parseExprAndCompare("1-simple.txt", "1-xml.txt");
    }SECTION("2") {
        TestParserUtils::parseExprAndCompare("2-simple.txt", "2-xml.txt");
    }SECTION("3") {
        TestParserUtils::parseExprAndCompare("3-simple.txt", "3-xml.txt");
    }
}

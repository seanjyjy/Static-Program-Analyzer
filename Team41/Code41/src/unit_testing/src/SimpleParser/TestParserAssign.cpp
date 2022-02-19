#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: assign") {
    SECTION("1") {
        TestParserUtils::parseAssignAndCompare("1-simple.txt", "1-xml.txt");
    }
    SECTION("complex") {
        TestParserUtils::parseAssignAndCompare("complex.simple", "complex.x");
    }
    SECTION("whitespace") {
        TestParserUtils::parseAssignAndCompare("whitespace.simple", "whitespace.x");
    }
}

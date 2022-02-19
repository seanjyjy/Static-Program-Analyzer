#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: read statement") {
    SECTION("1") {
        TestParserUtils::parseReadAndCompare("1-simple.txt", "1-xml.txt");
    }
    SECTION("whitespace") {
        TestParserUtils::parseReadAndCompare("whitespace.simple", "whitespace.x");
    }
}

#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: name") {
    SECTION("1") {
        TestParserUtils::parseNameAndCompare("1-simple.txt", "1-xml.txt");
    }SECTION("capitalized") {
        TestParserUtils::parseNameAndCompare("capitalized.simple", "capitalized.x");
    }SECTION("letters and digits") {
        TestParserUtils::parseNameAndCompare("letters-digits.simple", "letters-digits.x");
    }
}

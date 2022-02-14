#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: assign") {
    SECTION("1") {
        TestParserUtils::parseAssignAndCompare("1-simple.txt", "1-xml.txt");
    }
}

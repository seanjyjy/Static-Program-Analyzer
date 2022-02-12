#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: call") {
    SECTION("1") {
        TestParserUtils::parseCallAndCompare("1-simple.txt", "1-xml.txt");
    }
}

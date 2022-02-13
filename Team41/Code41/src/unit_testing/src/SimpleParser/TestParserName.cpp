#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: name") {
    SECTION("1") {
        TestParserUtils::parseNameAndCompare("1-simple.txt", "1-xml.txt");
    }
}

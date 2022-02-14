#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: constant") {
    SECTION("1") {
        TestParserUtils::parseConstAndCompare("1-simple.txt", "1-xml.txt");
    }
}

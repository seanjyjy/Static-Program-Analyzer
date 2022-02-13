#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: if statement") {
    SECTION("1") {
        TestParserUtils::parseIfAndCompare("1-simple.txt", "1-xml.txt");
    }
}

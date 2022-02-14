#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: program") {
    SECTION("1") {
        TestParserUtils::parseProgramAndCompare("1-simple.txt", "1-xml.txt");
    }
}

#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: while statement") {
    SECTION("1") {
        TestParserUtils::parseWhileAndCompare("1-simple.txt", "1-xml.txt");
    }
}

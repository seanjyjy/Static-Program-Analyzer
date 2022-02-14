#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: print statement") {
    SECTION("1") {
        TestParserUtils::parsePrintAndCompare("1-simple.txt", "1-xml.txt");
    }
}

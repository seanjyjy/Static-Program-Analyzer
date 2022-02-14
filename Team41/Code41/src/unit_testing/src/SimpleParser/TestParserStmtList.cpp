#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: statement list") {
    SECTION("1") {
        TestParserUtils::parseProcedureAndCompare("1-simple.txt", "1-xml.txt");
    }
}

#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: procedure") {
    SECTION("1") {
        TestParserUtils::parseProcedureAndCompare("1-simple.txt", "1-xml.txt");
    }
    SECTION("multiple procedures") {
        TestParserUtils::parseProgramAndCompare("multi-procedures.simple", "multi-procedures.x");
    }
}

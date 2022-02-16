#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: program") {
    SECTION("1") {
        TestParserUtils::parseProgramAndCompare("1-simple.txt", "1-xml.txt");
    }
    SECTION("multiple procedures") {
        TestParserUtils::parseProgramAndCompare("multi-procedures.simple", "multi-procedures.x");
    }
    SECTION("N0 F") {
        TestParserUtils::parseProgramAndCompare("n0f.simple", "n0f.x");
    }
    SECTION("N0 L") {
        TestParserUtils::parseProgramAndCompare("n0l.simple", "n0l.x");
    }
    SECTION("N0 M") {
        TestParserUtils::parseProgramAndCompare("n0m.simple", "n0m.x");
    }
    SECTION("N1 II F") {
        TestParserUtils::parseProgramAndCompare("n1iif.simple", "n1iif.x");
    }
    SECTION("N1 II L") {
        TestParserUtils::parseProgramAndCompare("n1iil.simple", "n1iil.x");
    }
    SECTION("N1 II M") {
        TestParserUtils::parseProgramAndCompare("n1iim.simple", "n1iim.x");
    }
    SECTION("N1 IW F") {
        TestParserUtils::parseProgramAndCompare("n1iwf.simple", "n1iwf.x");
    }
    SECTION("N1 IW L") {
        TestParserUtils::parseProgramAndCompare("n1iwl.simple", "n1iwl.x");
    }
    SECTION("N1 IW M") {
        TestParserUtils::parseProgramAndCompare("n1iwm.simple", "n1iwm.x");
    }
    SECTION("N1 WI F") {
        TestParserUtils::parseProgramAndCompare("n1wif.simple", "n1wif.x");
    }
    SECTION("N1 WI L") {
        TestParserUtils::parseProgramAndCompare("n1wil.simple", "n1wil.x");
    }
    SECTION("N1 WI M") {
        TestParserUtils::parseProgramAndCompare("n1wim.simple", "n1wim.x");
    }
    SECTION("N2 WW F") {
        TestParserUtils::parseProgramAndCompare("n2wwf.simple", "n2wwf.x");
    }
    SECTION("N2 WW L") {
        TestParserUtils::parseProgramAndCompare("n2wwl.simple", "n2wwl.x");
    }
    SECTION("N2 WW M") {
        TestParserUtils::parseProgramAndCompare("n2wwm.simple", "n2wwm.x");
    }
    SECTION("N2 II F") {
        TestParserUtils::parseProgramAndCompare("n2iif.simple", "n2iif.x");
    }
    SECTION("N2 II L") {
        TestParserUtils::parseProgramAndCompare("n2iil.simple", "n2iil.x");
    }
    SECTION("N2 II M") {
        TestParserUtils::parseProgramAndCompare("n2iim.simple", "n2iim.x");
    }
    SECTION("N2 IW F") {
        TestParserUtils::parseProgramAndCompare("n2iwf.simple", "n2iwf.x");
    }
    SECTION("N2 IW L") {
        TestParserUtils::parseProgramAndCompare("n2iwl.simple", "n2iwl.x");
    }
    SECTION("N2 IW M") {
        TestParserUtils::parseProgramAndCompare("n2iwm.simple", "n2iwm.x");
    }
    SECTION("N2 WI F") {
        TestParserUtils::parseProgramAndCompare("n2wif.simple", "n2wif.x");
    }
    SECTION("N2 WI L") {
        TestParserUtils::parseProgramAndCompare("n2wil.simple", "n2wil.x");
    }
    SECTION("N2 WI M") {
        TestParserUtils::parseProgramAndCompare("n2wim.simple", "n2wim.x");
    }
    SECTION("N3 WW F") {
        TestParserUtils::parseProgramAndCompare("n3wwf.simple", "n3wwf.x");
    }
    SECTION("N3 WW L") {
        TestParserUtils::parseProgramAndCompare("n3wwl.simple", "n3wwl.x");
    }
    SECTION("N3 WW M") {
        TestParserUtils::parseProgramAndCompare("n3wwm.simple", "n3wwm.x");
    }
    SECTION("N3 II F") {
        TestParserUtils::parseProgramAndCompare("n3iif.simple", "n3iif.x");
    }
    SECTION("N3 II L") {
        TestParserUtils::parseProgramAndCompare("n3iil.simple", "n3iil.x");
    }
    SECTION("N3 II M") {
        TestParserUtils::parseProgramAndCompare("n3iim.simple", "n3iim.x");
    }
    SECTION("N3 IW F") {
        TestParserUtils::parseProgramAndCompare("n3iwf.simple", "n3iwf.x");
    }
    SECTION("N3 IW L") {
        TestParserUtils::parseProgramAndCompare("n3iwl.simple", "n3iwl.x");
    }
    SECTION("N3 IW M") {
        TestParserUtils::parseProgramAndCompare("n3iwm.simple", "n3iwm.x");
    }
    SECTION("N3 WI F") {
        TestParserUtils::parseProgramAndCompare("n3wif.simple", "n3wif.x");
    }
    SECTION("N3 WI L") {
        TestParserUtils::parseProgramAndCompare("n3wil.simple", "n3wil.x");
    }
    SECTION("N3 WI M") {
        TestParserUtils::parseProgramAndCompare("n3wim.simple", "n3wim.x");
    }
}

#include "catch.hpp"
#include "TestParserUtils.h"

TEST_CASE("Parser: conditional expression") {
    SECTION("1") {
        TestParserUtils::parseCondExprAndCompare("1-simple.txt", "1-xml.txt");
    }SECTION("2") {
        TestParserUtils::parseCondExprAndCompare("2-simple.txt", "2-xml.txt");
    }SECTION("3") {
        TestParserUtils::parseCondExprAndCompare("3-simple.txt", "3-xml.txt");
    }SECTION("4") {
        TestParserUtils::parseCondExprAndCompare("4-simple.txt", "4-xml.txt");
    }SECTION("5") {
        TestParserUtils::parseCondExprAndCompare("5-simple.txt", "5-xml.txt");
    }SECTION("6") {
        TestParserUtils::parseCondExprAndCompare("6-simple.txt", "6-xml.txt");
    }SECTION("7") {
        TestParserUtils::parseCondExprAndCompare("7-simple.txt", "7-xml.txt");
    }SECTION("8") {
        TestParserUtils::parseCondExprAndCompare("8-simple.txt", "8-xml.txt");
    }SECTION("9") {
        TestParserUtils::parseCondExprAndCompare("9-simple.txt", "9-xml.txt");
    }SECTION("10") {
        TestParserUtils::parseCondExprAndCompare("10-simple.txt", "10-xml.txt");
    }
}

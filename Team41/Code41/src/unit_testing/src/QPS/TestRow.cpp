#include "catch.hpp"
#include "QPS/Table/Row.h"

using namespace std;

TEST_CASE("QPS: Row") {
    Row row;
    unordered_map<string, string> emptyRow;

    SECTION("empty row") {
        REQUIRE(row.size() == 0);
        REQUIRE_FALSE(row.hasColumn("a"));
        REQUIRE_THROWS(row.getValueAtColumn("a"));
        REQUIRE(row.getRow() == emptyRow);
    }

    SECTION("non empty row") {
        unordered_map<string, string> newRow;
        newRow.insert({"a", "2"});
        row.addEntry("a", "2");

        REQUIRE(row.size() == 1);

        REQUIRE(row.getRow() != emptyRow);
        REQUIRE(row.getRow() == newRow);

        REQUIRE(row.hasColumn("a"));
        REQUIRE_FALSE(row.hasColumn("b"));

        REQUIRE_THROWS(row.addEntry("a", "1"));
        REQUIRE_NOTHROW(row.addEntry("a", "2"));

        REQUIRE(row.getValueAtColumn("a") == "2");
        REQUIRE_THROWS(row.getValueAtColumn("b"));
    }
}
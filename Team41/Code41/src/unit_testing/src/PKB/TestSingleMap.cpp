#include "PKB/SingleMap.h"

#include "catch.hpp"
#include "../UnitTestUtility.h"
#include <unordered_set>

using namespace std;

enum TestKeys {
    TEST_KEY_1,
    TEST_KEY_2,
};

enum TestValues {
    TEST_VALUE_1,
    TEST_VALUE_2,
};

TEST_CASE("PKB: SingleMap") {
    SingleMap<TestKeys, TestValues> table;

    SECTION("empty table") {
        // Before adding key values
        REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_1));
        REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
        REQUIRE(table.keySize() == 0);
    }

    SECTION("adding values") {
        // Adding one key-value
        table.put(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1);

        REQUIRE(table.hasKey(TestKeys::TEST_KEY_1));
        REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_2));
        REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
        REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2));
        REQUIRE(table.keySize() == 1);

        // Adding to same key replaces value
        table.put(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2);

        REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
        REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2));
        REQUIRE(table.keySize() == 1);

        // Adding to diff key
        REQUIRE_NOTHROW(table.put(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1));

        REQUIRE(table.hasKey(TestKeys::TEST_KEY_1));
        REQUIRE(table.hasKey(TestKeys::TEST_KEY_2));
        REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1));
        REQUIRE(table.keySize() == 2);
    }

    SECTION("set equivalence") {
        unordered_set<TestKeys> comparisonSet;
        vector<pair<TestKeys, TestValues>> entrySet;
        entrySet.reserve(10);
        REQUIRE(table.keys() == comparisonSet);
        REQUIRE(sortAndCompareVectors(table.entries(), entrySet));

        // populating table
        table.put(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1);
        comparisonSet.insert(TestKeys::TEST_KEY_1);
        entrySet.push_back(make_pair(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
        REQUIRE(table.keys() == comparisonSet);
        REQUIRE(sortAndCompareVectors(table.entries(), entrySet));

        table.put(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1);
        REQUIRE_FALSE(table.keys() == comparisonSet);
        REQUIRE_FALSE(sortAndCompareVectors(table.entries(), entrySet));
        comparisonSet.insert(TestKeys::TEST_KEY_2);
        entrySet.push_back(make_pair(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1));
        REQUIRE(table.keys() == comparisonSet);
        REQUIRE(sortAndCompareVectors(table.entries(), entrySet));
    }
}

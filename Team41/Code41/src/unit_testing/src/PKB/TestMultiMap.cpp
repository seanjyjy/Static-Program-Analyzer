#include "PKB/SetMultiMap.h"
#include "../UnitTestUtility.h"
#include "catch.hpp"

using namespace std;

enum TestKeys {
    TEST_KEY_1,
    TEST_KEY_2,
};

enum TestValues {
    TEST_VALUE_1,
    TEST_VALUE_2,
};

TEST_CASE("MultiMap") {
    SetMultiMap<TestKeys, TestValues> table;

    SECTION("empty table") {
        // Before adding key values
        REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasKeyValue(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(table.get(TEST_KEY_1).size() == 0);
        REQUIRE(table.get(TEST_KEY_2).size() == 0);
        REQUIRE(table.keySize() == 0);
    }

    SECTION("adding values") {
        // Adding one key-value
        table.add(TEST_KEY_1, TEST_VALUE_1);

        REQUIRE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasKey(TEST_KEY_2));
        REQUIRE(table.hasKeyValue(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE_FALSE(table.hasKeyValue(TEST_KEY_1, TEST_VALUE_2));
        REQUIRE(table.get(TEST_KEY_1).size() == 1);
        REQUIRE(table.get(TEST_KEY_2).size() == 0);
        REQUIRE(table.keySize() == 1);

        // Adding to same key
        table.add(TEST_KEY_1, TEST_VALUE_2);

        REQUIRE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasKey(TEST_KEY_2));
        REQUIRE(table.hasKeyValue(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(table.hasKeyValue(TEST_KEY_1, TEST_VALUE_2));
        REQUIRE(table.get(TEST_KEY_1).size() == 2);
        REQUIRE(table.get(TEST_KEY_2).size() == 0);
        REQUIRE(table.keySize() == 1);

        // Adding to diff key
        table.add(TEST_KEY_2, TEST_VALUE_1);

        REQUIRE(table.hasKey(TEST_KEY_1));
        REQUIRE(table.hasKey(TEST_KEY_2));
        REQUIRE(table.hasKeyValue(TEST_KEY_2, TEST_VALUE_1));
        REQUIRE(table.get(TEST_KEY_1).size() == 2);
        REQUIRE(table.get(TEST_KEY_2).size() == 1);
        REQUIRE(table.keySize() == 2);
    }

    SECTION("set equivalence") {
        unordered_set<TestKeys> comparisonSet;
        vector<pair<TestKeys, TestValues>> entrySet;
        entrySet.reserve(10);
        REQUIRE(table.keys() == comparisonSet);
        REQUIRE(compareVectors(table.entries(),entrySet));

        // populating table
        unordered_set<TestValues> valueSet_1;
        valueSet_1.insert(TEST_VALUE_1);
        valueSet_1.insert(TEST_VALUE_2);
        table.put(TEST_KEY_1, valueSet_1);
        table.add(TEST_KEY_2, TEST_VALUE_1);

        // correct keys set
        comparisonSet.insert(TEST_KEY_1);
        comparisonSet.insert(TEST_KEY_2);
        entrySet.push_back(make_pair(TEST_KEY_1, TEST_VALUE_1));
        entrySet.push_back(make_pair(TEST_KEY_1, TEST_VALUE_2));
        entrySet.push_back(make_pair(TEST_KEY_2, TEST_VALUE_1));
        REQUIRE(table.keys() == comparisonSet);
        REQUIRE(compareVectors(table.entries(),entrySet));

        // correct values set
        unordered_set<TestValues> valueSet_2;
        valueSet_2.insert(TEST_VALUE_1);
        REQUIRE(table.get(TEST_KEY_1) == valueSet_1);
        REQUIRE(table.get(TEST_KEY_2) == valueSet_2);

        // overriding works
        table.put(TEST_KEY_1, unordered_set<TestValues>());
        REQUIRE_FALSE(table.get(TEST_KEY_1) == valueSet_1);
    }
}

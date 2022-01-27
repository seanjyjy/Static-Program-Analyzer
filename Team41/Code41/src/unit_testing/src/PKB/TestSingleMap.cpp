//
// Created by JinHao on 26/1/22.
//

#include "PKB/SingleMap.h"

#include "TNode.h"
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

TEST_CASE("SingleMap: empty table") {
    SingleMap<TestKeys, TestValues> table;

    // Before adding key values
    REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_1));
    REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
    REQUIRE(table.keySize() == 0);
}

TEST_CASE("SingleMap: adding values") {
    SingleMap<TestKeys, TestValues> table;

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

TEST_CASE("SingleMap: set equivalence") {
    SingleMap<TestKeys, TestValues> table;
    set<TestKeys> comparisonSet;
    set<pair<TestKeys, TestValues>> entrySet;
    REQUIRE(table.keys() == comparisonSet);
    REQUIRE(table.entries() == entrySet);

    // populating table
    table.put(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1);
    comparisonSet.insert(TestKeys::TEST_KEY_1);
    entrySet.insert(make_pair(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
    REQUIRE(table.keys() == comparisonSet);
    REQUIRE(table.entries() == entrySet);

    table.put(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1);
    REQUIRE_FALSE(table.keys() == comparisonSet);
    REQUIRE_FALSE(table.entries() == entrySet);
    comparisonSet.insert(TestKeys::TEST_KEY_2);
    entrySet.insert(make_pair(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1));
    REQUIRE(table.keys() == comparisonSet);
    REQUIRE(table.entries() == entrySet);
}
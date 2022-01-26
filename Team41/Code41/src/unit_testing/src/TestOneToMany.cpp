//
// Created by JinHao on 26/1/22.
//

#include <PKB/OneToMany.cpp>

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

TEST_CASE("OneToMany: empty table") {
    OneToMany<TestKeys, TestValues> table;

    // Before adding key values
    REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_1));
    REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
    REQUIRE(table.get(TestKeys::TEST_KEY_1).size() == 0);
    REQUIRE(table.get(TestKeys::TEST_KEY_2).size() == 0);
    REQUIRE(table.keySize() == 0);
}

TEST_CASE("OneToMany: adding values") {
    OneToMany<TestKeys, TestValues> table;

    // Adding one key-value
    table.add(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1);

    REQUIRE(table.hasKey(TestKeys::TEST_KEY_1));
    REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_2));
    REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
    REQUIRE_FALSE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2));
    REQUIRE(table.get(TestKeys::TEST_KEY_1).size() == 1);
    REQUIRE(table.get(TestKeys::TEST_KEY_2).size() == 0);
    REQUIRE(table.keySize() == 1);

    // Adding to same key
    table.add(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2);

    REQUIRE(table.hasKey(TestKeys::TEST_KEY_1));
    REQUIRE_FALSE(table.hasKey(TestKeys::TEST_KEY_2));
    REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_1));
    REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_1, TestValues::TEST_VALUE_2));
    REQUIRE(table.get(TestKeys::TEST_KEY_1).size() == 2);
    REQUIRE(table.get(TestKeys::TEST_KEY_2).size() == 0);
    REQUIRE(table.keySize() == 1);

    // Adding to diff key
    table.add(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1);

    REQUIRE(table.hasKey(TestKeys::TEST_KEY_1));
    REQUIRE(table.hasKey(TestKeys::TEST_KEY_2));
    REQUIRE(table.hasKeyValue(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1));
    REQUIRE(table.get(TestKeys::TEST_KEY_1).size() == 2);
    REQUIRE(table.get(TestKeys::TEST_KEY_2).size() == 1);
    REQUIRE(table.keySize() == 2);
}

TEST_CASE("OneToMany: set equivalence") {
    OneToMany<TestKeys, TestValues> table;
    set<TestKeys> comparisonSet;
    REQUIRE(table.keys() == comparisonSet);

    // populating table
    set<TestValues> valueSet_1;
    valueSet_1.insert(TestValues::TEST_VALUE_1);
    valueSet_1.insert(TestValues::TEST_VALUE_2);
    table.put(TestKeys::TEST_KEY_1, valueSet_1);
    table.add(TestKeys::TEST_KEY_2, TestValues::TEST_VALUE_1);

    // correct keys set
    comparisonSet.insert(TestKeys::TEST_KEY_1);
    comparisonSet.insert(TestKeys::TEST_KEY_2);
    REQUIRE(table.keys() == comparisonSet);

    // correct values set
    set<TestValues> valueSet_2;
    valueSet_2.insert(TestValues::TEST_VALUE_1);
    REQUIRE(table.get(TestKeys::TEST_KEY_1) == valueSet_1);
    REQUIRE(table.get(TestKeys::TEST_KEY_2) == valueSet_2);

    // overriding works
    table.put(TestKeys::TEST_KEY_1, std::set<TestValues>());
    REQUIRE_FALSE(table.get(TestKeys::TEST_KEY_1) == valueSet_1);
}
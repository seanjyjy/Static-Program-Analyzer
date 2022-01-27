//
// Created by JinHao on 26/1/22.
//

#include "catch.hpp"
#include "PKB/ManyToMany.h"

using namespace std;

enum TestKeys {
    TEST_KEY_1,
    TEST_KEY_2,
    TEST_KEY_3,
    TEST_KEY_4,
};

enum TestValues {
    TEST_VALUE_1,
    TEST_VALUE_2,
    TEST_VALUE_3,
    TEST_VALUE_4,
};

TEST_CASE("ManyToMany: empty table") {
    ManyToMany<TestKeys, TestValues> table("Many2Many_TestTable");

    // Before adding
    REQUIRE(table.keySize() == 0);
    REQUIRE(table.valSize() == 0);
    REQUIRE(table.getKeys() == set<TestKeys>());
    REQUIRE(table.getValues() == set<TestValues>());
    REQUIRE(table.getValuesFromKey(TEST_KEY_1) == set<TestValues>());
    REQUIRE(table.getKeyFromValue(TEST_VALUE_1) == set<TestKeys>());
    REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
    REQUIRE_FALSE(table.hasVal(TEST_VALUE_1));
    REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));
}

TEST_CASE("ManyToMany: adding relation") {
    const string tableName = "Many2Many_TestTable";
    ManyToMany<TestKeys, TestValues> table(tableName);

    // Adding key-values
    TestKeys testKeys[] = {TEST_KEY_1, TEST_KEY_2, TEST_KEY_3};
    TestValues testValues[] = {TEST_VALUE_1, TEST_VALUE_2, TEST_VALUE_3};
    set<TestKeys> keySet;
    set<TestValues> valueSet;
    int i = 0;
    for (TestKeys key: testKeys) {
        table.addMapping(key, testValues[i]);
        keySet.insert(key);
        valueSet.insert(testValues[i]);
        i++;
    }
    REQUIRE(table.getKeys() == keySet);
    REQUIRE(table.getValues() == valueSet);

    // no error thrown if mapping already exists
    REQUIRE_NOTHROW(table.addMapping(TEST_KEY_1, TEST_VALUE_1));

    // query
    REQUIRE(table.hasKey(TEST_KEY_1));
    REQUIRE(table.hasVal(TEST_VALUE_1));
    REQUIRE_FALSE(table.hasKey(TEST_KEY_4));
    REQUIRE_FALSE(table.hasVal(TEST_VALUE_4));
    REQUIRE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));
    REQUIRE(table.hasMapping(TEST_KEY_3, TEST_VALUE_3));
    REQUIRE_FALSE(table.hasMapping(TEST_KEY_2, TEST_VALUE_3));
    REQUIRE_FALSE(table.hasMapping(TEST_KEY_4, TEST_VALUE_4));
    REQUIRE(table.valSize() == 3);
    REQUIRE(table.keySize() == 3);

    set<TestValues> assignValSet;
    assignValSet.insert(TEST_VALUE_2);
    assignValSet.insert(TEST_VALUE_4);
    set<TestKeys> assignKeySet;
    assignKeySet.insert(TEST_KEY_2);

    // TEST_KEY_2 --> {TEST_VALUE_1, TEST_VALUE_4}
    REQUIRE_NOTHROW(table.addMapping(TEST_KEY_2, TEST_VALUE_4));
    REQUIRE(table.getValuesFromKey(TEST_KEY_2) == assignValSet);
    REQUIRE(table.getKeyFromValue(TEST_VALUE_4) == assignKeySet);
    REQUIRE(table.valSize() == 4);
    REQUIRE(table.keySize() == 3);

    set<TestValues> assignValSet2;
    assignValSet2.insert(TEST_VALUE_4);
    set<TestKeys> assignKeySet2;
    assignKeySet2.insert(TEST_KEY_2);
    assignKeySet2.insert(TEST_KEY_4);

    // {TEST_KEY_2, TEST_KEY_4} <-- TEST_VALUE_4
    REQUIRE_NOTHROW(table.addMapping(TEST_KEY_4, TEST_VALUE_4));
    REQUIRE(table.getValuesFromKey(TEST_KEY_4) == assignValSet2);
    REQUIRE(table.getKeyFromValue(TEST_VALUE_4) == assignKeySet2);
    REQUIRE(table.valSize() == 4);
    REQUIRE(table.keySize() == 4);
}

#include "catch.hpp"
#include "PKB/OneToMany.h"

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

TEST_CASE("OneToMany") {
    const string tableName = "TestTable";
    OneToMany<TestKeys, TestValues> table("TestTable");

    SECTION("empty state") {
        // Before adding
        REQUIRE(table.keySize() == 0);
        REQUIRE(table.valSize() == 0);
        REQUIRE(table.getKeys() == set<TestKeys>());
        REQUIRE(table.getValues() == set<TestValues>());
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == set<TestValues>());
        REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasVal(TEST_VALUE_1));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));
    }

    SECTION("adding relation") {
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

        // throw error if a value have multiple key
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_2, TEST_VALUE_1),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) && Catch::Contains("Multiple keys"));
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_4, TEST_VALUE_1),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) && Catch::Contains("Multiple keys"));
        // don't throw if mapping already exists
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

        // TEST_KEY_2 --> {TEST_VALUE_1, TEST_VALUE_4}
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_2, TEST_VALUE_4));
        REQUIRE(table.getValuesFromKey(TEST_KEY_2) == assignValSet);
        REQUIRE(table.getKeyFromValue(TEST_VALUE_4) == TEST_KEY_2);
        REQUIRE(table.valSize() == 4);
        REQUIRE(table.keySize() == 3);
    }

    SECTION("immutability") {
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == set<TestValues>());
        table.getValuesFromKey(TEST_KEY_1).insert(TEST_VALUE_1);
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == set<TestValues>());

        REQUIRE(table.getKeys() == set<TestKeys>());
        table.getKeys().insert(TEST_KEY_1);
        REQUIRE(table.getKeys() == set<TestKeys>());

        REQUIRE(table.getValues() == set<TestValues>());
        table.getValues().insert(TEST_VALUE_1);
        REQUIRE(table.getValues() == set<TestValues>());

        REQUIRE(table.getEntries() == set<pair<TestKeys,  TestValues>>());
        table.getEntries().insert(make_pair(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(table.getEntries() == set<pair<TestKeys,  TestValues>>());
    }
}

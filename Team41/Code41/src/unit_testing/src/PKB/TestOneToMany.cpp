#include "catch.hpp"
#include "PKB/OneToMany.h"
#include "../UnitTestUtility.h"

using namespace std;

enum TestKeys {
    DEFAULT_KEY,
    TEST_KEY_1,
    TEST_KEY_2,
    TEST_KEY_3,
    TEST_KEY_4,
};

enum TestValues {
    DEFAULT_VALUE,
    TEST_VALUE_1,
    TEST_VALUE_2,
    TEST_VALUE_3,
    TEST_VALUE_4,
};

TEST_CASE("PKB: OneToMany") {
    const string tableName = "TestTable";
    OneToMany<TestKeys, TestValues> table("TestTable");

    SECTION("empty state") {
        // Before adding
        REQUIRE(table.keySize() == 0);
        REQUIRE(table.valSize() == 0);
        REQUIRE(table.getKeys() == unordered_set<TestKeys>());
        REQUIRE(table.getValues() == unordered_set<TestValues>());
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());
        REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasVal(TEST_VALUE_1));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));

        REQUIRE(table.getKeyFromValue(TEST_VALUE_1) == DEFAULT_KEY);
    }

    SECTION("adding relation") {
        // Adding key-values
        TestKeys testKeys[] = {TEST_KEY_1, TEST_KEY_2, TEST_KEY_3};
        TestValues testValues[] = {TEST_VALUE_1, TEST_VALUE_2, TEST_VALUE_3};
        unordered_set<TestKeys> keySet;
        unordered_set<TestValues> valueSet;
        vector<pair<TestKeys, TestValues>> entrySet;
        int i = 0;
        for (TestKeys key: testKeys) {
            TestValues val = testValues[i];
            table.addMapping(key, val);
            keySet.insert(key);
            valueSet.insert(val);
            entrySet.push_back(make_pair(key, val));
            i++;
        }
        REQUIRE(table.getKeys() == keySet);
        REQUIRE(table.getValues() == valueSet);
        REQUIRE(sortAndCompareVectors(table.getEntries(), entrySet));

        // throw error if a value have multiple key
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_2, TEST_VALUE_1),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) &&
                            Catch::Contains("[One-Many]") && Catch::Contains("Multiple keys"));
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_4, TEST_VALUE_1),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) &&
                            Catch::Contains("[One-Many]") && Catch::Contains("Multiple keys"));
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

        unordered_set<TestValues> assignValSet;
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
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());
        table.getValuesFromKey(TEST_KEY_1).insert(TEST_VALUE_1);
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());

        REQUIRE(table.getKeys() == unordered_set<TestKeys>());
        table.getKeys().insert(TEST_KEY_1);
        REQUIRE(table.getKeys() == unordered_set<TestKeys>());

        REQUIRE(table.getValues() == unordered_set<TestValues>());
        table.getValues().insert(TEST_VALUE_1);
        REQUIRE(table.getValues() == unordered_set<TestValues>());

        REQUIRE(sortAndCompareVectors(table.getEntries(), vector<pair<TestKeys, TestValues>>()));
        table.getEntries().push_back(make_pair(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(sortAndCompareVectors(table.getEntries(), vector<pair<TestKeys, TestValues>>()));
    }
}

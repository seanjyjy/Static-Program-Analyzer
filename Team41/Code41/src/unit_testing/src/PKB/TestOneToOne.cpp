#include "catch.hpp"
#include "PKB/Relations/OneToOne.h"
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

TEST_CASE("PKB: OneToOne") {
    const string tableName = "TestTable";
    OneToOne<TestKeys, TestValues> table(tableName);

    SECTION("empty table") {
        // Before adding
        REQUIRE(table.keySize() == 0);
        REQUIRE(table.valSize() == 0);
        REQUIRE(table.getKeys() == unordered_set<TestKeys>());
        REQUIRE(table.getValues() == unordered_set<TestValues>());
        REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasVal(TEST_VALUE_1));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));

        REQUIRE(table.getValFromKey(TEST_KEY_1) == DEFAULT_VALUE);
        REQUIRE(table.getKeyFromValue(TEST_VALUE_1) == DEFAULT_KEY);
    }

    SECTION("adding relation") {
        // Adding key-values
        unordered_set<TestKeys> keySet;
        unordered_set<TestValues> valueSet;
        vector<pair<TestKeys, TestValues>> entrySet;
        TestKeys testKeys[] = {TEST_KEY_1, TEST_KEY_2};
        TestValues testValues[] = {TEST_VALUE_1, TEST_VALUE_2};
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

        // query
        REQUIRE(table.hasKey(TEST_KEY_1));
        REQUIRE(table.hasVal(TEST_VALUE_1));
        REQUIRE_FALSE(table.hasKey(TEST_KEY_4));
        REQUIRE_FALSE(table.hasVal(TEST_VALUE_4));
        REQUIRE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(table.hasMapping(TEST_KEY_2, TEST_VALUE_2));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_2));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_4, TEST_VALUE_4));
        REQUIRE(table.valSize() == 2);
        REQUIRE(table.keySize() == 2);

        REQUIRE(table.getValFromKey(TEST_KEY_2) == TEST_VALUE_2);
        REQUIRE(table.getValFromKey(TEST_KEY_1) == TEST_VALUE_1);
        REQUIRE(table.getKeyFromValue(TEST_VALUE_2) == TEST_KEY_2);
    }

    SECTION("edge cases") {
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_1, TEST_VALUE_1));

        // throw error if mapping a mapped key to another variable
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_1, TEST_VALUE_2),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) &&
                            Catch::Contains("[One-One]") && Catch::Contains("Key already mapped"));

        // throw error if mapping a key to a mapped value
        REQUIRE_THROWS_WITH(table.addMapping(TEST_KEY_2, TEST_VALUE_1),
                            Catch::Contains("[PKB]") && Catch::Contains(tableName) &&
                            Catch::Contains("[One-One]") && Catch::Contains("Value already mapped"));

        // don't throw for valid mapping
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_2, TEST_VALUE_2));

        // don't throw if mapping already exists
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_1, TEST_VALUE_1));
    }

    SECTION("immutability") {
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

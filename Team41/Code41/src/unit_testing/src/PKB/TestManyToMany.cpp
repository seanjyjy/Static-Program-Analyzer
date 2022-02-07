#include "catch.hpp"
#include "PKB/ManyToMany.h"
#include "../UnitTestUtility.h"

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

TEST_CASE("PKB: ManyToMany") {
    const string tableName = "Many2Many_TestTable";
    ManyToMany<TestKeys, TestValues> table(tableName);

    SECTION("empty table") {
        // Before adding
        REQUIRE(table.keySize() == 0);
        REQUIRE(table.valSize() == 0);
        REQUIRE(table.getKeys() == unordered_set<TestKeys>());
        REQUIRE(table.getValues() == unordered_set<TestValues>());
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());
        REQUIRE(table.getKeysFromValue(TEST_VALUE_1) == unordered_set<TestKeys>());
        REQUIRE_FALSE(table.hasKey(TEST_KEY_1));
        REQUIRE_FALSE(table.hasVal(TEST_VALUE_1));
        REQUIRE_FALSE(table.hasMapping(TEST_KEY_1, TEST_VALUE_1));
    }

    SECTION("adding relation") {
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

        unordered_set<TestValues> assignValSet;
        assignValSet.insert(TEST_VALUE_2);
        assignValSet.insert(TEST_VALUE_4);
        unordered_set<TestKeys> assignKeySet;
        assignKeySet.insert(TEST_KEY_2);

        // TEST_KEY_2 --> {TEST_VALUE_1, TEST_VALUE_4}
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_2, TEST_VALUE_4));
        REQUIRE(table.getValuesFromKey(TEST_KEY_2) == assignValSet);
        REQUIRE(table.getKeysFromValue(TEST_VALUE_4) == assignKeySet);
        REQUIRE(table.valSize() == 4);
        REQUIRE(table.keySize() == 3);

        unordered_set<TestValues> assignValSet2;
        assignValSet2.insert(TEST_VALUE_4);
        unordered_set<TestKeys> assignKeySet2;
        assignKeySet2.insert(TEST_KEY_2);
        assignKeySet2.insert(TEST_KEY_4);

        // {TEST_KEY_2, TEST_KEY_4} <-- TEST_VALUE_4
        REQUIRE_NOTHROW(table.addMapping(TEST_KEY_4, TEST_VALUE_4));
        REQUIRE(table.getValuesFromKey(TEST_KEY_4) == assignValSet2);
        REQUIRE(table.getKeysFromValue(TEST_VALUE_4) == assignKeySet2);
        REQUIRE(table.valSize() == 4);
        REQUIRE(table.keySize() == 4);
    }

    SECTION("immutability") {
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());
        table.getValuesFromKey(TEST_KEY_1).insert(TEST_VALUE_1);
        REQUIRE(table.getValuesFromKey(TEST_KEY_1) == unordered_set<TestValues>());

        REQUIRE(table.getKeysFromValue(TEST_VALUE_1) == unordered_set<TestKeys>());
        table.getKeysFromValue(TEST_VALUE_1).insert(TEST_KEY_1);
        REQUIRE(table.getKeysFromValue(TEST_VALUE_1) == unordered_set<TestKeys>());

        REQUIRE(table.getKeys() == unordered_set<TestKeys>());
        table.getKeys().insert(TEST_KEY_1);
        REQUIRE(table.getKeys() == unordered_set<TestKeys>());

        REQUIRE(table.getValues() == unordered_set<TestValues>());
        table.getValues().insert(TEST_VALUE_1);
        REQUIRE(table.getValues() == unordered_set<TestValues>());

        REQUIRE(sortAndCompareVectors(table.getEntries(), vector<pair<TestKeys,  TestValues>>()));
        table.getEntries().push_back(make_pair(TEST_KEY_1, TEST_VALUE_1));
        REQUIRE(sortAndCompareVectors(table.getEntries(), vector<pair<TestKeys,  TestValues>>()));
    }
}

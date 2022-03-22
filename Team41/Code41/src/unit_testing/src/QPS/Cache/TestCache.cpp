#include "catch.hpp"
#include "QPS/Cache/Cache.h"

using namespace std;

TEST_CASE("Cache") {
    Cache cache;

    string keys[] = {"k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7"};
    string values[] = {"v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7"};

    vector<vector<string>> fixedTestCase = {
            {values[1], values[5], values[6]},
            {values[2], values[3], values[4], values[5], values[6], values[7]},
            {values[1], values[2]},
            {values[0], values[4], values[6], values[7]},
            {values[0], values[3], values[5]},
            {values[6]},
            {values[0], values[1], values[2], values[5]}
    };

    for (int i = 0; i < (int) fixedTestCase.size(); i++) {
        for (auto& value : fixedTestCase.at(i)) {
            cache.registerForwardMapping(keys[i], value);
            cache.registerBooleanMapping(keys[i], value);
            cache.registerBackwardMapping(keys[i], value);
        }
    }

    SECTION("Test forward mapping") {
        REQUIRE(cache.getForwardMapping(keys[0]) == unordered_set<string>({values[1], values[5], values[6]}));
        REQUIRE(cache.getForwardMapping(keys[1]) == unordered_set<string>({values[2], values[3], values[4], values[5], values[6], values[7]}));
        REQUIRE(cache.getForwardMapping(keys[2]) == unordered_set<string>({values[1], values[2]}));
        REQUIRE(cache.getForwardMapping(keys[3]) == unordered_set<string>({values[0], values[4], values[6], values[7]}));
        REQUIRE(cache.getForwardMapping(keys[4]) == unordered_set<string>({values[0], values[3], values[5]}));
        REQUIRE(cache.getForwardMapping(keys[5]) == unordered_set<string>({values[6]}));
        REQUIRE(cache.getForwardMapping(keys[6]) == unordered_set<string>({values[0], values[1], values[2], values[5]}));
        REQUIRE(cache.getForwardMapping(keys[7]).empty());
    }

    SECTION("Test backward mapping") {
        REQUIRE(cache.getBackwardMapping(keys[0]) == unordered_set<string>({values[1], values[5], values[6]}));
        REQUIRE(cache.getBackwardMapping(keys[1]) == unordered_set<string>({values[2], values[3], values[4], values[5], values[6], values[7]}));
        REQUIRE(cache.getBackwardMapping(keys[2]) == unordered_set<string>({values[1], values[2]}));
        REQUIRE(cache.getBackwardMapping(keys[3]) == unordered_set<string>({values[0], values[4], values[6], values[7]}));
        REQUIRE(cache.getBackwardMapping(keys[4]) == unordered_set<string>({values[0], values[3], values[5]}));
        REQUIRE(cache.getBackwardMapping(keys[5]) == unordered_set<string>({values[6]}));
        REQUIRE(cache.getBackwardMapping(keys[6]) == unordered_set<string>({values[0], values[1], values[2], values[5]}));
        REQUIRE(cache.getBackwardMapping(keys[7]).empty());
    }

    // Test a subset of missing (no need all)
    SECTION("Test boolean mapping") {
        for (auto& value : {values[1], values[5], values[6]}) {
            REQUIRE(cache.getBooleanMapping(keys[0], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[0], values[0]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[0], values[2]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[0], values[3]));

        for (auto& value : {values[2], values[3], values[4], values[5], values[6], values[7]}) {
            REQUIRE(cache.getBooleanMapping(keys[1], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[1], values[0]));

        for (auto& value : {values[1], values[2]}) {
            REQUIRE(cache.getBooleanMapping(keys[2], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[2], values[0]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[2], values[5]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[2], values[6]));

        for (auto& value : {values[0], values[4], values[6], values[7]}) {
            REQUIRE(cache.getBooleanMapping(keys[3], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[3], values[1]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[3], values[2]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[3], values[3]));

        for (auto& value : {values[0], values[3], values[5]}) {
            REQUIRE(cache.getBooleanMapping(keys[4], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[4], values[1]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[4], values[2]));

        for (auto& value : {values[6]}) {
            REQUIRE(cache.getBooleanMapping(keys[5], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[5], values[1]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[5], values[2]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[5], values[3]));

        for (auto& value : {values[0], values[1], values[2], values[5]}) {
            REQUIRE(cache.getBooleanMapping(keys[6], value));
        }
        REQUIRE_FALSE(cache.getBooleanMapping(keys[6], values[4]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[6], values[6]));

        REQUIRE_FALSE(cache.getBooleanMapping(keys[7], values[0]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[7], values[2]));
        REQUIRE_FALSE(cache.getBooleanMapping(keys[7], values[3]));
    }
}
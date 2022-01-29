#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

/**
 * Mapping of a unique key to a set of values
 *
 * @tparam K the key of the relation
 * @tparam V the val of the relation
 */
template<typename K, typename V>
class SetMultiMap {
private:
    unordered_map<K, unordered_set<V>> mapping;
    unordered_set<K> keySet;

public:
    /**
     * Get many relationships by key
     *
     * @param key the key to query by
     * @return the set of values that are related to key
     */
    unordered_set<V> get(K key);

    /**
     * Get the set of keys used in table
     *
     * @return set of keys
     */
    unordered_set<K> keys();

    /**
     * Gets all unique entries used in the table
     *
     * @return list of key-value pair
     */
    vector<pair<K, V>> entries();

    /**
     * Get the number of keys in table
     *
     * @return number of keys
     */
    size_t keySize();

    /**
     * Checks if key exists in table
     *
     * @param targetKey the key to query
     * @return true if exists, false otherwise
     */
    bool hasKey(K targetKey);

    /**
     * Checks if key-value pair exists in table
     *
     * @param key the key to query
     * @param val the value to query
     * @return true if the pair exists, false otherwise
     */
    bool hasKeyValue(K key, V val);

    /**
     * Add value into one-to-many relationship
     *
     * @param key the key that the value belongs to
     * @param val The value to add in
     */
    void add(K key, V val);

    /**
     * Puts the one-to-many relation for the specified key or replace if relation exists
     *
     * @param key the target key
     * @param values the set of values to add into the table
     */
    void put(K key, unordered_set<V> values);
};

#include "SetMultiMap.tpp"

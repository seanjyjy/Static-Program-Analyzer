#pragma once

#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * Mapping of a unique key to a value
 *
 * @tparam K the key of the relation
 * @tparam V the val of the relation
 */
template<typename K, typename V>
class SingleMap {
private:
    map<K, V> mapping;
    set<K> keySet;

public:
    /**
     * Get the one-to-one relationship by key
     *
     * @param key the key to query by
     * @return the value that is mapped to specified key
     */
    V get(K key);

    /**
     * Get the set of keys used in table
     *
     * @return set of keys
     */
    set<K> keys();

    /**
     * Gets all entries used in the table
     *
     * @return set of key-value pair
     */
    set<pair<K, V>> entries();

    /**
     * Get the number of keys in table
     *
     * @return number of keys
     */
    int keySize();

    /**
     * Checks if key-value pair exists in table
     *
     * @param key the key to query
     * @param val the value to query
     * @return true if the pair exists, false otherwise
     */
    bool hasKeyValue(K key, V val);

    /**
     * Checks if key exists in table
     *
     * @param targetKey the key to query
     * @return true if exists, false otherwise
     */
    bool hasKey(K targetKey);

    /**
     * Puts the key-value pair into table
     *
     * @param key the target key
     * @param values the value that is mapped to target key
     */
    void put(K key, V val);
};

#include "SingleMap.tpp"


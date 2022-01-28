#pragma once

#include <string>

#include "MultiMap.h"

using namespace std;

template<class K, class V>
class ManyToMany {
private:
    string relationName;
    MultiMap<K, V> keyToValues;
    MultiMap<V, K> valuesToKey;
public:
    explicit ManyToMany(const string &relationName);

    /**
     * Gets set of values that belongs to the specified key
     *
     * @param key the target key
     * @return the set of values
     */
    unordered_set<V> getValuesFromKey(K key);

    /**
     * Gets set of keys that the value belong to
     *
     * @param value the value to query by
     * @return the key that value belongs to
     */
    unordered_set<K> getKeysFromValue(V value);

    /**
     * Checks if key exists in table
     *
     * @param targetKey the key to query
     * @return true if exists, false otherwise
     */
    bool hasKey(K targetKey);

    /**
     * Checks if val exists in table
     *
     * @param targetVal the val to query
     * @return true if exists, false otherwise
     */
    bool hasVal(V targetVal);

    /**
     * Checks if the mapping exists in many-to-many relation
     *
     * @param key the key to query
     * @param val the value to query
     * @return true if exists, false otherwise
     */
    bool hasMapping(K key, V val);

    /**
     * Adds the key-value mapping into the table
     *
     * @param key the key value to set
     * @param val the value mapped to specified key
     */
    void addMapping(K key, V val);

    /**
     * Gets all keys added to the relation table
     *
     * @return set of keys
     */
    unordered_set<K> getKeys();

    /**
     * Gets all values added to the relation table
     *
     * @return set of values
     */
    unordered_set<V> getValues();

    /**
     * Gets all entries in the relation table
     *
     * @return list of key-value pair
     */
    vector<pair<K, V>> getEntries();

    /**
     * Gets the number of keys stored in table
     *
     * @return number of keys
     */
    int keySize();

    /**
     * Gets the number of values stored in table
     *
     * @return number of vals
     */
    int valSize();
};

#include "ManyToMany.tpp"

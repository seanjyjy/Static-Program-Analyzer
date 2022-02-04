#pragma once

#include "SingleMap.h"
#include "SingleMap.h"

#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

/**
 * A K key can have only one V value and
 * each V value can only have one K key
 *
 * @tparam K the key of the relation
 * @tparam V the val of the relation
 */
template<typename K, typename V>
class OneToOne {
private:
    string relationName;
    SingleMap<K, V> keyToVal;
    SingleMap<V, K> valToKey;
public:
    explicit OneToOne(const string &relationName);

    /**
     * Gets the value that belongs to the specified key
     *
     * @param key the target key
     * @return the val mapped to key
     * @throws error if key does not exist
     */
    V getValFromKey(K key);


    /**
     * Gets the key that the value belong to
     *
     * @param value the value to query by
     * @return the key that value belongs to
     */
    K getKeyFromValue(V value);

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
     * Checks if the mapping exists in one-to-one relation
     *
     * @param key the key to query
     * @param val the value to query
     * @return true if exists, false otherwise
     */
    bool hasMapping(K key, V val);

    /**
     * Adds the key-value mapping into the table
     * Checks if mapping does not violate one-to-one constraints
     *
     * Note: invalid insertion will result in the key->val, val->key relation not being upheld,
     * hence error is thrown
     *
     * @param key the key value to set
     * @param val the value mapped to specified key
     * @throws error if insertion results in val having multiple keys or key having multiple values
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
     * @return set of key-value pair
     */
    vector<pair<K, V>> getEntries();

    /**
     * Gets the number of keys stored in table
     *
     * @return number of keys
     */
    size_t keySize();

    /**
     * Gets the number of values stored in table
     *
     * @return number of vals
     */
    size_t valSize();
};

#include "OneToOne.tpp"


//
// Created by JinHao on 26/1/22.
//

#ifndef SPA_ONETOONE_H
#define SPA_ONETOONE_H

#include <map>
#include <set>
#include <string>

using namespace std;

template <typename K, typename V>
class OneToOne {
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
     * Get the number of keys in table
     *
     * @return number of keys
     */
    int keySize();

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
     * Puts the key-value pair into table
     *
     * @param key the target key
     * @param values the value that is mapped to target key
     */
    void put(K key, V val);
};

#endif //SPA_ONETOONE_H

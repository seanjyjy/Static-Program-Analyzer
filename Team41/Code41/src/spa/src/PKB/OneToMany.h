//
// Created by JinHao on 26/1/22.
//
#pragma once

#ifndef SPA_ONETOMANY_H
#define SPA_ONETOMANY_H

#include <map>
#include <set>
#include <string>

using namespace std;

template <typename K, typename V>
class OneToMany {
private:
    map<K, set<V>> mapping;
    set<K> keySet;

public:
    /**
     * Get many relationships by key
     *
     * @param key the key to query by
     * @return the set of values that are related to key
     */
    set<V> get(K key);

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
    void put(K key, set<V> values);
};


#endif //SPA_ONETOMANY_H

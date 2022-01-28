//
// Created by JinHao on 26/1/22.
//
#include "MultiMap.h"

using namespace std;

template<class K, class V>
set<V> MultiMap<K, V>::get(K key) {
    return mapping[key];
}

template<class K, class V>
set<K> MultiMap<K, V>::keys() {
    return keySet;
}

template<class K, class V>
set<pair<K, V>> MultiMap<K, V>::entries() {
    set<pair<K, V>> resultSet;
    for (K key : keySet) {
        for(V val : mapping[key]) {
            pair<K, V> newPair = make_pair(key, val);
            resultSet.insert(newPair);
        }
    }
    return resultSet;
}

template<class K, class V>
int MultiMap<K, V>::keySize() {
    return keySet.size();
}

template<class K, class V>
bool MultiMap<K, V>::hasKey(K targetKey) {
    return keySet.find(targetKey) != keySet.end();
}

template<class K, class V>
bool MultiMap<K, V>::hasKeyValue(K key, V val) {
    set<V> values = mapping[key];
    return values.find(val) != values.end();
}

template<class K, class V>
void MultiMap<K, V>::add(K key, V val) {
    set<V> *values = &mapping[key];
    values->insert(val);
    keySet.insert(key);
}

template<class K, class V>
void MultiMap<K, V>::put(K key, set<V> values) {
    this->mapping[key] = values;
    keySet.insert(key);
}

//
// Created by JinHao on 26/1/22.
//
#include "SingleMap.h"

using namespace std;

template<class K, class V>
V SingleMap<K, V>::get(K key) {
    return mapping[key];
}

template<class K, class V>
set<K> SingleMap<K, V>::keys() {
    return keySet;
}

template<class K, class V>
set<pair<K, V>> SingleMap<K, V>::entries() {
    set<pair<K, V>> resultSet;
    for (K key : keySet) {
        resultSet.insert(make_pair(key, mapping[key]));
    }
    return resultSet;
}

template<class K, class V>
int SingleMap<K, V>::keySize() {
    return keySet.size();
}

template<class K, class V>
bool SingleMap<K, V>::hasKeyValue(K key, V val) {
    typename map<K, V>::iterator it = mapping.find(key);
    return it != mapping.end() && it->second == val;
}

template<class K, class V>
bool SingleMap<K, V>::hasKey(K targetKey) {
    return keySet.find(targetKey) != keySet.end();
}

template<class K, class V>
void SingleMap<K, V>::put(K key, V val) {
    mapping[key] = val;
    keySet.insert(key);
}

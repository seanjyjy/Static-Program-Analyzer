//
// Created by JinHao on 26/1/22.
//

#include "OneToOne.h"

using namespace std;

template <class K, class V>
V OneToOne<K, V>::get(K key) {
    return mapping[key];
}

template <class K, class V>
set<K> OneToOne<K, V>::keys() {
    return keySet;
}

template <class K, class V>
int OneToOne<K, V>::keySize() {
    return keySet.size();
}

template <class K, class V>
bool OneToOne<K, V>::hasKeyValue(K key, V val) {
    typename std::map<K,V>::iterator it = mapping.find(key);
    return it != mapping.end() && it->second == val;
}

template <class K, class V>
bool OneToOne<K, V>::hasKey(K targetKey) {
    return keySet.find(targetKey) != keySet.end();;
}

template <class K, class V>
void OneToOne<K, V>::put(K key, V val) {
    mapping[key] = val;
    keySet.insert(key);
}

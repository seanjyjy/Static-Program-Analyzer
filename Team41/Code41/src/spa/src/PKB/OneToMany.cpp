//
// Created by JinHao on 26/1/22.
//

#include "OneToMany.h"

using namespace std;

template <class K, class V>
set<V> OneToMany<K, V>::get(K key) {
    return mapping[key];
}

template <class K, class V>
set<K> OneToMany<K, V>::keys() {
    return keySet;
}

template <class K, class V>
int OneToMany<K, V>::keySize() {
    return keySet.size();
}

template <class K, class V>
bool OneToMany<K, V>::hasKeyValue(K key, V val) {
    set<V>* values = &mapping[key];
    return values->find(val) != values->end();
}

template <class K, class V>
bool OneToMany<K, V>::hasKey(K targetKey) {
    return keySet.find(targetKey) != keySet.end();;
}

template <class K, class V>
void OneToMany<K, V>::add(K key, V val) {
    set<V>* values = &mapping[key];
    values->insert(val);
    keySet.insert(key);
}

template <class K, class V>
void OneToMany<K, V>::put(K key, set<V> values) {
    this->mapping[key] = values;
    keySet.insert(key);
}

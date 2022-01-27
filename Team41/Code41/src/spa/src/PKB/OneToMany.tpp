//
// Created by JinHao on 26/1/22.
//

#include "OneToMany.h"

using namespace std;

template<class K, class V>
OneToMany<K, V>::OneToMany(const string &relationName) {
    this->relationName = relationName;
}

template<class K, class V>
set<V> OneToMany<K, V>::getValuesFromKey(K key) {
    return keyToValues.get(key);
}

template<class K, class V>
K OneToMany<K, V>::getKeyFromValue(V value) {
    return valuesToKey.get(value);
}

template<class K, class V>
bool OneToMany<K, V>::hasKey(K targetKey) {
    return keyToValues.hasKey(targetKey);
}

template<class K, class V>
bool OneToMany<K, V>::hasVal(V targetVal) {
    return valuesToKey.hasKey(targetVal);
}

template<class K, class V>
bool OneToMany<K, V>::hasMapping(K key, V val) {
    return keyToValues.hasKeyValue(key, val);
}


template<class K, class V>
void OneToMany<K, V>::addMapping(K key, V val) {
    // checks that if val exists in table, make sure that it is only mapped from one unique key
    if (valuesToKey.hasKey(val) && !valuesToKey.hasKeyValue(val, key)) {
        throw runtime_error("[PKB][" + relationName + "] Multiple keys detected for specified val");
    }
    keyToValues.add(key, val);
    valuesToKey.put(val, key);
}

template<class K, class V>
set<K> OneToMany<K, V>::getKeys() {
    return keyToValues.keys();
}

template<class K, class V>
set<V> OneToMany<K, V>::getValues() {
    return valuesToKey.keys();
}

template<class K, class V>
int OneToMany<K, V>::keySize() {
    return keyToValues.keySize();
}

template<class K, class V>
int OneToMany<K, V>::valSize() {
    return valuesToKey.keySize();
}


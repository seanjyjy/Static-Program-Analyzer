#include "ManyToMany.h"

template<class K, class V>
ManyToMany<K, V>::ManyToMany() : relationName("GenericMany2ManyTable") {}

template<class K, class V>
ManyToMany<K, V>::ManyToMany(const string &relationName) {
    this->relationName = relationName;
}

template<class K, class V>
unordered_set<V> ManyToMany<K, V>::getValuesFromKey(K key) {
    return keyToValues.get(key);
}

template<class K, class V>
unordered_set<K> ManyToMany<K, V>::getKeysFromValue(V value) {
    return valuesToKey.get(value);
}

template<class K, class V>
bool ManyToMany<K, V>::hasKey(K targetKey) {
    return keyToValues.hasKey(targetKey);
}

template<class K, class V>
bool ManyToMany<K, V>::hasVal(V targetVal) {
    return valuesToKey.hasKey(targetVal);
}


template<class K, class V>
bool ManyToMany<K, V>::hasMapping(K key, V val) {
    return keyToValues.hasKeyValue(key, val);
}

template<class K, class V>
void ManyToMany<K, V>::addMapping(K key, V val) {
    if (!hasMapping(key, val)) {
        keyToValues.add(key, val);
        valuesToKey.add(val, key);
        entries.push_back(make_pair(key, val));
    }
}

template<class K, class V>
unordered_set<K> ManyToMany<K, V>::getKeys() {
    return keyToValues.keys();
}


template<class K, class V>
unordered_set<V> ManyToMany<K, V>::getValues() {
    return valuesToKey.keys();
}

template<typename K, typename V>
vector<pair<K, V>> ManyToMany<K, V>::getEntries() {
    return entries;
}

template<class K, class V>
size_t ManyToMany<K, V>::size() {
    return entries.size();
}

template<class K, class V>
size_t ManyToMany<K, V>::keySize() {
    return keyToValues.keySize();
}

template<class K, class V>
size_t ManyToMany<K, V>::valSize() {
    return valuesToKey.keySize();
}


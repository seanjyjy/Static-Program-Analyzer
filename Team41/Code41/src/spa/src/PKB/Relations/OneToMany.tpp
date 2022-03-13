#include "OneToMany.h"
#include <stdexcept>

using namespace std;

template<class K, class V>
OneToMany<K, V>::OneToMany(const string &relationName) {
    this->relationName = relationName;
}

template<class K, class V>
unordered_set<V> OneToMany<K, V>::getValuesFromKey(K key) {
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
    // already mapped
    if (this->hasMapping(key, val)) {
        return;
    }

    // checks that if val exists in table, make sure that it is only mapped from one unique key
    if (this->hasVal(val)) {
        throw runtime_error("[PKB][" + relationName + "][One-Many] Multiple keys detected for specified val");
    }

    if (!hasMapping(key, val)) {
        keyToValues.add(key, val);
        valuesToKey.put(val, key);
        entries.push_back(make_pair(key, val));
    }
}

template<class K, class V>
unordered_set<K> OneToMany<K, V>::getKeys() {
    return keyToValues.keys();
}

template<class K, class V>
unordered_set<V> OneToMany<K, V>::getValues() {
    return valuesToKey.keys();
}

template<typename K, typename V>
vector<pair<K, V>> OneToMany<K, V>::getEntries() {
    return entries;
}

template<class K, class V>
size_t OneToMany<K, V>::keySize() {
    return keyToValues.keySize();
}

template<class K, class V>
size_t OneToMany<K, V>::valSize() {
    return valuesToKey.keySize();
}


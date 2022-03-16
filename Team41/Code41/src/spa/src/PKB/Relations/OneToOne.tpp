#include "OneToOne.h"

using namespace std;

template<class K, class V>
OneToOne<K, V>::OneToOne(const string &relationName) {
    this->relationName = relationName;
}

template<class K, class V>
V OneToOne<K, V>::getValFromKey(K key) {
    return keyToVal.get(key);
}

template<class K, class V>
K OneToOne<K, V>::getKeyFromValue(V value) {
    return valToKey.get(value);
}

template<class K, class V>
bool OneToOne<K, V>::hasKey(K targetKey) {
    return keyToVal.hasKey(targetKey);
}

template<class K, class V>
bool OneToOne<K, V>::hasVal(V targetVal) {
    return valToKey.hasKey(targetVal);
}

template<class K, class V>
bool OneToOne<K, V>::hasMapping(K key, V val) {
    return keyToVal.hasKeyValue(key, val);
}


template<class K, class V>
void OneToOne<K, V>::addMapping(K key, V val) {
    if (this->hasMapping(key, val)) {
        return;
    }
    // checks that if val exists in table, make sure that it is only mapped from one unique key
    if (this->hasKey(key)) {
        throw runtime_error("[PKB][" + relationName + "][One-One] Key already mapped to another val");
    }
    // checks that if val exists in table, make sure that it is only mapped from one unique key
    if (this->hasVal(val)) {
        throw runtime_error("[PKB][" + relationName + "][One-One] Value already mapped from another key");
    }
    if (!hasMapping(key, val)) {
        keyToVal.put(key, val);
        valToKey.put(val, key);
        entries.push_back(make_pair(key, val));
    }
}

template<class K, class V>
unordered_set<K> OneToOne<K, V>::getKeys() {
    return keyToVal.keys();
}

template<class K, class V>
unordered_set<V> OneToOne<K, V>::getValues() {
    return valToKey.keys();
}

template<typename K, typename V>
vector<pair<K, V>> OneToOne<K, V>::getEntries() {
    return entries;
}

template<class K, class V>
size_t OneToOne<K, V>::size() {
    return entries.size();
}

template<typename K, typename V>
size_t OneToOne<K, V>::size(K key) {
    return hasKey(move(key)) ? 1 : 0;
}

template<class K, class V>
size_t OneToOne<K, V>::keySize() {
    return keyToVal.keySize();
}

template<class K, class V>
size_t OneToOne<K, V>::valSize() {
    return valToKey.keySize();
}


#include "MultiMap.h"
#include "vector"

using namespace std;

template<class K, class V>
unordered_set<V> MultiMap<K, V>::get(K key) {
    return mapping[key];
}

template<class K, class V>
unordered_set<K> MultiMap<K, V>::keys() {
    return keySet;
}

template<class K, class V>
vector<pair<K, V>> MultiMap<K, V>::entries() {
    vector<pair<K, V>> resultSet;
    resultSet.reserve(mapping.size());
    for (auto &it: mapping) {
        K key = it.first;
        for (V val: it.second) {
            resultSet.push_back(make_pair(key, val));
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
    unordered_set<V> values = mapping[key];
    return values.find(val) != values.end();
}

template<class K, class V>
void MultiMap<K, V>::add(K key, V val) {
    unordered_set<V> *values = &mapping[key];
    values->insert(val);
    keySet.insert(key);
}

template<class K, class V>
void MultiMap<K, V>::put(K key, unordered_set<V> values) {
    this->mapping[key] = values;
    keySet.insert(key);
}

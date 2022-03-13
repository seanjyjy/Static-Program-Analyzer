#include "SingleMap.h"

using namespace std;

template<class K, class V>
V SingleMap<K, V>::get(K key) {
    return hasKey(key) ? mapping[key] : V();
}

template<class K, class V>
unordered_set<K> SingleMap<K, V>::keys() {
    return keySet;
}

template<class K, class V>
size_t SingleMap<K, V>::keySize() {
    return keySet.size();
}

template<class K, class V>
bool SingleMap<K, V>::hasKeyValue(K key, V val) {
    auto it = mapping.find(key);
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

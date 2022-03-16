#include "SetMultiMap.h"
#include "vector"

using namespace std;

template<class K, class V>
unordered_set<V> SetMultiMap<K, V>::get(K key) {
    return hasKey(key) ? mapping[key] : unordered_set<V>();
}

template<class K, class V>
unordered_set<K> SetMultiMap<K, V>::keys() {
    return keySet;
}

template<typename K, typename V>
size_t SetMultiMap<K, V>::size(K key) {
    if (!hasKey(key)) {
        return 0;
    }
    return mapping[key].size();
}

template<class K, class V>
size_t SetMultiMap<K, V>::keySize() {
    return keySet.size();
}

template<class K, class V>
bool SetMultiMap<K, V>::hasKey(K targetKey) {
    return keySet.find(targetKey) != keySet.end();
}

template<class K, class V>
bool SetMultiMap<K, V>::hasKeyValue(K key, V val) {
    if (!hasKey(key)) return false;
    unordered_set<V> values = mapping[key];
    return values.find(val) != values.end();
}

template<class K, class V>
void SetMultiMap<K, V>::add(K key, V val) {
    unordered_set<V> *values = &mapping[key];
    values->insert(val);
    keySet.insert(key);
}

template<class K, class V>
void SetMultiMap<K, V>::put(K key, unordered_set<V> values) {
    this->mapping[key] = values;
    keySet.insert(key);
}

#pragma once
#include <memory>
#include <string>
#include <unordered_map>

template <class T, typename valueType, typename hash, typename equals> class Cache {
public:
    void addResult(std::shared_ptr<T> key, valueType values) {
        cache[key] = values;
    }

    valueType findResult(std::shared_ptr<T> key) {
        if (cache.find(key) == cache.end()) {
            return valueType();
        }

        return cache[key];
    }

    void clearCache() {
        cache.clear();
    }

private:
    std::unordered_map<std::shared_ptr<T>, valueType, hash, equals> cache;
};

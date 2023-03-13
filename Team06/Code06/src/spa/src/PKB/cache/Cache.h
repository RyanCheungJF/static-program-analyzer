#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#pragma once
#include <memory>
#include <unordered_map>
#include <string>

template<class T, typename hash, typename equals>
class Cache {
public:
    void addResult(std::shared_ptr<T> key, std::vector<std::vector<std::string>> values) {
        cache[key] = values;
    }

    std::vector<std::vector<std::string>> findResult(std::shared_ptr<T> key) {
        if (cache.find(key) == cache.end()) {
            return std::vector<std::vector<std::string>>();
        }

        return cache[key];
    }

private:
    std::unordered_map<std::shared_ptr<T>, std::vector<std::vector<std::string>>, hash, equals> cache;
};

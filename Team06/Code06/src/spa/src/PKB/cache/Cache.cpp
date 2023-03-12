#include "Cache.h"

void Cache::addResult(shared_ptr<Relationship> rs, std::vector<std::string> results) {
    cache[rs] = results;
}

std::vector<std::string> Cache::findResult(shared_ptr<Relationship> rs) {
    if (cache.find(rs) == cache.end()) {
        return std::vector<std::string>();
    }

    return cache[rs];
}
#include <unordered_map>

#include "../../QPS/entities/Relationship.h"

template<> struct std::hash<shared_ptr<Relationship>> {
    std::size_t operator()(shared_ptr<Relationship> const& rs) const {
        std::size_t h1 = std::hash<RelationshipType>{}(rs->getType());
        std::size_t h2 = std::hash<std::string>{}(rs->getParameters()[0].getTypeString());
        std::size_t h3 = std::hash<std::string>{}(rs->getParameters()[1].getTypeString());

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

class Cache {
public:
    void addResult(shared_ptr<Relationship> rs, std::vector<std::string> results);
    std::vector<std::string> findResult(shared_ptr<Relationship> rs);

private:
    std::unordered_map<shared_ptr<Relationship>, std::vector<std::string>> cache;
};
#include "../../QPS/entities/Relationship.h"
#include "Cache.h"

struct relationshipHash {
    std::size_t operator()(shared_ptr<Relationship> const& rs) const {
        std::size_t h1 = std::hash<RelationshipType>{}(rs->getType());
        std::size_t h2 = std::hash<Parameter>{}(rs->getParameters()[0]);
        std::size_t h3 = std::hash<Parameter>{}(rs->getParameters()[1]);

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct relationshipEquals {
    bool operator()(shared_ptr<Relationship> const& rs1, shared_ptr<Relationship> const& rs2) const {
        bool check1 = rs1->getType() == rs2->getType();
        bool check2 = rs1->getParameters()[0].getValue() == rs2->getParameters()[0].getValue();
        bool check3 = rs1->getParameters()[1].getValue() == rs2->getParameters()[1].getValue();

        return check1 && check2 && check3;
    }
};

class RelationshipCache
    : public Cache<Relationship, std::vector<std::vector<std::string>>, relationshipHash, relationshipEquals> {};

#include "../../QPS/entities/Relationship.h"
#include "Cache.h"

struct relationshipHash {
    std::size_t operator()(shared_ptr<Relationship> const& rs) const {
        std::vector<ParameterType> paramTypes = rs->getParameterTypes();

        std::size_t h1 = std::hash<RelationshipType>{}(rs->getType());
        std::size_t h2 = std::hash<ParameterType>{}(paramTypes[0]);
        std::size_t h3 = std::hash<ParameterType>{}(paramTypes[1]);

        std::vector<Parameter> params = rs->getParameters();
        if (paramTypes[0] == ParameterType::FIXED_INT || paramTypes[0] == ParameterType::FIXED_STRING) {
            h2 = std::hash<Parameter>{}(params[0]);
        }

        if (paramTypes[1] == ParameterType::FIXED_INT || paramTypes[1] == ParameterType::FIXED_STRING) {
            h3 = std::hash<Parameter>{}(params[1]);
        }

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct relationshipEquals {
    bool operator()(shared_ptr<Relationship> const& rs1, shared_ptr<Relationship> const& rs2) const {
        std::vector<ParameterType> paramTypes1 = rs1->getParameterTypes();
        std::vector<ParameterType> paramTypes2 = rs2->getParameterTypes();

        bool check1 = rs1->getType() == rs2->getType();
        bool check2 = paramTypes1[0] == paramTypes2[0];
        bool check3 = paramTypes1[1] == paramTypes2[1];

        std::vector<Parameter> params1 = rs1->getParameters();
        std::vector<Parameter> params2 = rs2->getParameters();
        if (paramTypes1[0] == ParameterType::FIXED_INT || paramTypes1[0] == ParameterType::FIXED_STRING) {
            check2 = params1[0] == params2[0];
        }

        if (paramTypes1[1] == ParameterType::FIXED_INT || paramTypes1[1] == ParameterType::FIXED_STRING) {
            check3 = params1[1] == params2[1];
        }

        return check1 && check2 && check3;
    }
};

class RelationshipCache
    : public Cache<Relationship, std::vector<std::vector<std::string>>, relationshipHash, relationshipEquals> {};

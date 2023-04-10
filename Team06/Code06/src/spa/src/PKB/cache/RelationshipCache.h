#include "../../QPS/entities/Relationship.h"
#include "Cache.h"

struct relationshipHash {
    std::size_t operator()(const std::shared_ptr<Relationship>& rs) const {
        std::vector<ParameterType> paramTypes = rs->getParameterTypes();
        std::vector<std::string> paramValues = rs->getParameterValues();

        std::size_t h1 = std::hash<RelationshipType>{}(rs->getType());
        std::size_t h2 = std::hash<ParameterType>{}(paramTypes[0]);
        std::size_t h3 = std::hash<ParameterType>{}(paramTypes[1]);

        std::vector<Parameter> params = rs->getParameters();
        if (params[0].isFixedInt() || params[0].isFixedStringType()) {
            std::size_t temp = std::hash<std::string>{}(paramValues[0]);
            h2 = h2 ^ (temp << 1);
        }

        if (params[1].isFixedInt() || params[1].isFixedStringType()) {
            std::size_t temp = std::hash<std::string>{}(paramValues[1]);
            h3 = h3 ^ (temp << 1);
        }

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct relationshipEquals {
    bool operator()(const shared_ptr<Relationship>& rs1, const shared_ptr<Relationship>& rs2) const {
        std::vector<ParameterType> paramTypes1 = rs1->getParameterTypes();
        std::vector<ParameterType> paramTypes2 = rs2->getParameterTypes();

        bool check1 = rs1->getType() == rs2->getType();
        bool check2 = paramTypes1[0] == paramTypes2[0];
        bool check3 = paramTypes1[1] == paramTypes2[1];

        std::vector<Parameter> params1 = rs1->getParameters();
        std::vector<Parameter> params2 = rs2->getParameters();
        if (params1[0].isFixedInt() || params1[0].isFixedStringType()) {
            check2 = params1[0] == params2[0];
        }

        if (params1[1].isFixedInt() || params1[1].isFixedStringType()) {
            check3 = params1[1] == params2[1];
        }

        return check1 && check2 && check3;
    }
};

class RelationshipCache : public Cache<shared_ptr<Relationship>, std::vector<std::vector<std::string>>,
                                       relationshipHash, relationshipEquals> {};

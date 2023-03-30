#include "../../QPS/entities/Comparison.h"
#include "Cache.h"

struct comparisonHash {
    std::size_t operator()(const shared_ptr<Comparison>& comp) const {
        std::pair<ParameterType, ParameterType> paramTypes = comp->getParameterTypes();
        std::pair<AttributeType, AttributeType> attrTypes = comp->getParameterAttributes();

        std::size_t h1 = std::hash<RelationshipType>{}(paramTypes.first);
        std::size_t h2 = std::hash<ParameterType>{}(paramTypes[0]);
        std::size_t h3 = std::hash<ParameterType>{}(paramTypes[1]);

        std::vector<Parameter> params = rs->getParameters();
        if (params[0].isFixedInt() || params[0].isFixedStringType()) {
            h2 = std::hash<Parameter>{}(params[0]);
        }

        if (params[1].isFixedInt() || params[1].isFixedStringType()) {
            h3 = std::hash<Parameter>{}(params[1]);
        }

        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

struct comparisonEquals {
    bool operator()(const shared_ptr<Comparison>& comp1, const shared_ptr<Comparison>& comp2) const {
        std::vector<ParameterType> paramTypes1 = comp1->getParameterTypes();
        std::vector<ParameterType> paramTypes2 = comp2->getParameterTypes();

        bool check1 = comp1->getType() == comp2->getType();
        bool check2 = paramTypes1[0] == paramTypes2[0];
        bool check3 = paramTypes1[1] == paramTypes2[1];

        std::vector<Parameter> params1 = comp1->getParameters();
        std::vector<Parameter> params2 = comp2->getParameters();
        if (params1[0].isFixedInt() || params1[0].isFixedStringType()) {
            check2 = params1[0] == params2[0];
        }

        if (params1[1].isFixedInt() || params1[1].isFixedStringType()) {
            check3 = params1[1] == params2[1];
        }

        return check1 && check2 && check3;
    }
};

class ComparisonCache
    : public Cache<Comparison, std::vector<std::vector<std::string>>, comparisonHash, comparisonEquals> {};
